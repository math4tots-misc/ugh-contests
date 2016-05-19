#include <vector>
#include <map>
#include <set>
#include <functional>

#include <iostream>
#include <assert.h>

#x err

class Graph {
public:
  typedef long NodeId;
  typedef long EdgeId;
  typedef long Size;
  typedef double Weight;

  struct Edge {
    const EdgeId id;
    const NodeId from;
    const NodeId to;
    Weight weight;

    Edge(EdgeId i, NodeId f, NodeId t, Weight w):
        id(i), from(f), to(t), weight(w) {}
  };

  Graph(Size n): nnodes(n), next_edge_id(1) {}

  // ** Tier 1: mutable methods that need to seriously worry about
  // internal consistency.

  // Creates a new edges with given parameters and returns
  // id of this new edge.
  EdgeId add_edge(NodeId from, NodeId to, Weight weight) {
    EdgeId id = next_edge_id++;
    edges.emplace(id, Edge(id, from, to, weight));
    adjacency_matrix[from][to].insert(id);
    adjacency_matrix2[to][from].insert(id);
    return id;
  }

  // Deletes edge with given id.
  // Returns true if edge was found and deleted.
  // Returns false and does nothing if no such edge was found.
  bool delete_edge(EdgeId edge_id) {
    auto edge_iter = edges.find(edge_id);
    if (edge_iter == edges.end()) {
      return false;
    }
    Edge& e = edge_iter->second;
    adjacency_matrix[e.from][e.to].erase(e.id);
    adjacency_matrix2[e.to][e.from].erase(e.id);
    edges.erase(edge_iter);
    return true;
  }

  EdgeId set_edge_weight(EdgeId e, Weight w) {
    edges.find(e)->second.weight = w;
    return e;
  }

  // ** Tier 2: Mutable methods that depend on only Tier 1 methods
  // to perform mutations.

  EdgeId set_edge(NodeId from, NodeId to, Weight weight) {
    std::vector<EdgeId> matching_edge_ids = edge_ids(from, to);
    if (matching_edge_ids.empty()) {
      return add_edge(from, to, weight);
    }
    return set_edge_weight(matching_edge_ids[0], weight);
  }

  template <class Iterable>
  void delete_edges(const Iterable& edge_ids) {
    for (EdgeId id: edge_ids) {
      delete_edge(id);
    }
  }

  void delete_edges(NodeId from, NodeId to) {
    delete_edges(edge_ids(from, to));
  }

  // ** Tier 3: Const methods.

  Size number_of_nodes() const {
    return nnodes;
  }

  std::vector<EdgeId> all_edge_ids() const {
    std::vector<EdgeId> ids;
    for (auto& pair: edges) {
      ids.push_back(pair.first);
    }
    return ids;  // NRVO -Wpessimizing-move
  }

  std::vector<EdgeId> edge_ids(NodeId from, NodeId to) const {
    std::vector<EdgeId> ids;
    auto iter1 = adjacency_matrix.find(from);
    if (iter1 != adjacency_matrix.end()) {
      auto edges_from_from = iter1->second;
      auto iter2 = edges_from_from.find(to);
      if (iter2 != edges_from_from.end()) {
        for (EdgeId edge_id: iter2->second) {
          ids.push_back(edge_id);
        }
      }
    }
    return ids;  // NRVO -Wpessimizing-move
  }

  std::vector<EdgeId> edge_ids_from(NodeId from) const {
    std::vector<EdgeId> ids;
    auto iter1 = adjacency_matrix.find(from);
    if (iter1 != adjacency_matrix.end()) {
      auto edges_from_from = iter1->second;
      for (auto& pair: edges_from_from) {
        for (EdgeId e: pair.second) {
          ids.push_back(e);
        }
      }
    }
    return ids;  // NRVO -Wpessimizing-move
  }

  std::vector<EdgeId> edge_ids_to(NodeId to) const {
    std::vector<EdgeId> ids;
    auto iter1 = adjacency_matrix2.find(to);
    if (iter1 != adjacency_matrix2.end()) {
      auto edges_from_from = iter1->second;
      for (auto& pair: edges_from_from) {
        for (EdgeId e: pair.second) {
          ids.push_back(e);
        }
      }
    }
    return ids;  // NRVO -Wpessimizing-move
  }

  Weight edge_weight(NodeId from, NodeId to, Weight def) const {
    std::vector<EdgeId> matching_edge_ids = edge_ids(from, to);
    if (matching_edge_ids.empty()) {
      return def;
    }
    return edges.find(matching_edge_ids[0])->second.weight;
  }

  bool connected(NodeId from, NodeId to) const {
    return edge_weight(from, to, 0) > 0;
  }

private:

  const Size nnodes;
  EdgeId next_edge_id;

  // EdgeId id -> Edge
  std::map<EdgeId, Edge> edges;

  // NodeId from -> NodeId to -> {EdgeId}
  std::map<NodeId, std::map<NodeId, std::set<EdgeId>>> adjacency_matrix;

  // NodeId to -> NodeId from -> {EdgeId}
  std::map<NodeId, std::map<NodeId, std::set<EdgeId>>> adjacency_matrix2;
};

using namespace std;

int main() {
  {
    Graph g(10);
    assert(!g.connected(0, 1));
    assert(g.edge_weight(0, 1, -33) == -33);
    g.set_edge(0, 1, 78);
    assert(g.connected(0, 1));
    assert(g.edge_weight(0, 1, -33) == 78);
    g.delete_edges(0, 1);
    assert(!g.connected(0, 1));
    assert(g.edge_weight(0, 1, -33) == -33);
  }
  cout << "pass" << endl;
}
