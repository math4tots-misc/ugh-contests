/* graph.cc

NOTE: Requires C++11

g++ --std=c++11 -Wall -Werror graph.cc && ./a.out

Simple graph library for programming contests.

Some version of this used in:
  * usaco/milk6 (find_min_cut)
  * usaco/frameup (find_all_topological_orderings)

If you add any features here, make sure to add tests in 'graph_sanity_check'!
*/

#x graph.cc

#include <assert.h>
#include <iostream>

#include <algorithm>
#include <deque>
#include <vector>

struct DefaultGraphOptions {
  typedef double WeightType;
};

template <class GraphOptions=DefaultGraphOptions>
struct Graph {
  typedef typename GraphOptions::WeightType Weight;
  typedef int EdgeId;
  typedef int NodeId;

  struct Edge {
    const EdgeId id;
    const NodeId from, to;
    Weight weight;
    Edge(EdgeId id, NodeId from, NodeId to, Weight weight):
        id(id), from(from), to(to), weight(weight) {}
  };

  Graph(int N): N(N), ids_of_edges_from(N) {}

  const Edge& edge(EdgeId id) { return edges[id]; }

  EdgeId add_edge(NodeId from, NodeId to, Weight weight) {
    EdgeId id = edges.size();
    edges.push_back(Edge(id, from, to, weight));
    ids_of_edges_from[from].push_back(id);
    return id;
  }

  EdgeId set_edge(NodeId from, NodeId to, Weight weight) {
    for (EdgeId edge_id: ids_of_edges_from[from]) {
      Edge& edge = edges[edge_id];
      if (edge.to == to) {
        edge.weight = weight;
        return edge.id;
      }
    }
    return add_edge(from, to, weight);
  }

  // Looks for a path from source to sink using only edges with positive
  // weights and using as few edges as possible.
  bool find_path_bfs(NodeId source, NodeId sink,
                     std::vector<EdgeId>* out) const {
    out->clear();
    std::vector<EdgeId> from(N, -1);
    from[source] = -2;
    std::deque<NodeId> queue(1, source);
    while (!queue.empty()) {
      NodeId x = queue.front();
      queue.pop_front();
      for (auto edge_id: ids_of_edges_from[x]) {
        const Edge& edge = edges[edge_id];
        if (edge.weight > 0 && from[edge.to] == -1) {
          from[edge.to] = edge_id;
          if (edge.to == sink) {
            NodeId i = sink;
            while (i != source) {
              const Edge& edge = edges[from[i]];
              out->push_back(edge.id);
              i = edge.from;
            }
            return true;
          }
          queue.push_back(edge.to);
        }
      }
    }
    return false;
  }

  // Runs floodfill from source using only edges with positive
  // weights.
  void flood_fill(NodeId source, std::vector<bool>* flags) const {
    flags->assign(N, false);
    (*flags)[source] = true;
    std::vector<NodeId> stack(1, source);
    while (!stack.empty()) {
      NodeId a = stack.back();
      stack.pop_back();
      for (auto edge_id: ids_of_edges_from[a]) {
        const Edge& edge = edges[edge_id];
        if (!(*flags)[edge.to] && edge.weight > 0) {
          (*flags)[edge.to] = true;
          stack.push_back(edge.to);
        }
      }
    }
  }

  // Increases the weight of some edge going from a to b.
  // If no such edge already exists, a new edge is created.
  void increase_weight(NodeId a, NodeId b, Weight weight) {
    for (auto edge_id: ids_of_edges_from[a]) {
      Edge& edge = edges[edge_id];
      if (edge.to == b) {
        edge.weight += weight;
        return;
      }
    }
    // If we've finished the above for loop without returning,
    // we could find no such edge.
    add_edge(a, b, weight);
  }

  // In addition to finding the max flow from source to sink,
  // this method will mutate 'this' into a residual graph.
  Weight find_max_flow(NodeId source, NodeId sink) {
    Weight max_flow = 0;
    for (;;) {
      // Try to find a path from source to sink in this residual
      // graph.
      std::vector<EdgeId> path;
      if (!find_path_bfs(source, sink, &path))
        break;

      Weight flow = edges[path[0]].weight;
      for (auto edge_id: path)
        flow = flow < edges[edge_id].weight ? flow : edges[edge_id].weight;

      for (auto edge_id: path) {
        Edge& edge = edges[edge_id];
        edge.weight -= flow;
        increase_weight(edge.to, edge.from, flow);
      }

      max_flow += flow;
    }
    return max_flow;
  }

  Weight find_min_cut(NodeId source, NodeId sink,
                      std::vector<EdgeId>* out) const {
    out->clear();
    Graph residual_graph = *this;
    Weight total_cost = residual_graph.find_max_flow(source, sink);

    std::vector<bool> flags;
    residual_graph.flood_fill(source, &flags);

    for (auto& edge: edges)
      if (flags[edge.from] && !flags[edge.to])
        out->push_back(edge.id);

    return total_cost;
  }

  // Creates an adjacency list looking only at edges with positive weight.
  void create_adjacency_list(std::vector<std::vector<NodeId>>* out) const {
    out->assign(N, std::vector<NodeId>());
    for (auto& edge: edges)
      if (edge.weight > 0)
        (*out)[edge.from].push_back(edge.to);
  }

  // Find all possible orderings of the nodes such that A -> B implies that
  // node A comes before node B.
  // There must be no cycles.
  void find_all_topological_orderings(
      std::vector<std::vector<NodeId>>* out) const {
    out->clear();
    std::vector<std::vector<NodeId>> adjlist;
    create_adjacency_list(&adjlist);

    std::vector<int> en(N, 0);
    for (NodeId i = 0; i < N; i++)
      for (NodeId neighbor: adjlist[i])
        en[neighbor]++;

    std::vector<NodeId> stage;
    find_all_topological_orderings_helper(adjlist, &en, &stage, out);
  }

private:
  void find_all_topological_orderings_helper(
      const std::vector<std::vector<NodeId>>& adjlist,
      std::vector<int>* en, std::vector<NodeId>* stage,
      std::vector<std::vector<NodeId>>* out) const {
    if ((int) stage->size() == N) {
      out->push_back(*stage);
      return;
    }

    for (NodeId i = 0; i < N; i++) {
      if ((*en)[i] == 0 &&
          std::find(stage->begin(), stage->end(), i) == stage->end()) {
        for (NodeId neighbor: adjlist[i])
          (*en)[neighbor]--;
        stage->push_back(i);
        find_all_topological_orderings_helper(adjlist, en, stage, out);
        stage->pop_back();
        for (NodeId neighbor: adjlist[i])
          (*en)[neighbor]++;
      }
    }
  }

  int N;  // number of nodes in this graph.
  std::vector<Edge> edges;
  std::vector<std::vector<EdgeId>> ids_of_edges_from;
};

using namespace std;

void graph_sanity_check() {
  // Test flood_fill
  {
    Graph<> graph(10);
    graph.add_edge(0, 1, 5);
    graph.add_edge(1, 2, 5);
    graph.add_edge(2, 1, 1);
    graph.add_edge(2, 4, 1);
    graph.add_edge(1, 5, -1);
    graph.add_edge(1, 6, 0);

    std::vector<bool> flags;
    graph.flood_fill(0, &flags);
    assert(flags[0]);  // test that self is marked.
    assert(flags[1]);
    assert(flags[2]);  // test with 1 -> 2 -> 1 cycle.
    assert(!flags[3]);  // test no edge case.
    assert(flags[4]);  // test that 0 -> 1 -> 2 -> 4 works.
    assert(!flags[5]);  // test non-positive edges are ignored.
    assert(!flags[6]);

    graph.flood_fill(1, &flags);
    assert(!flags[0]);
  }

  // Test find_path_bfs
  {
    Graph<> graph(10);
    std::vector<Graph<>::EdgeId> path;

    assert(graph.add_edge(0, 1, 1) == 0);
    assert(graph.find_path_bfs(0, 1, &path));
    assert(path.size() == 1);
    assert(path[0] == 0);

    assert(!graph.find_path_bfs(1, 0, &path));
    assert(path.size() == 0);

    assert(graph.add_edge(1, 0, 1) == 1);
    assert(graph.find_path_bfs(1, 0, &path));
    assert(path.size() == 1);
    assert(path[0] == 1);

    // fail path finding when they are not connected.
    assert(!graph.find_path_bfs(0, 5, &path));
    assert(path.size() == 0);

    // multiple edges in path
    assert(graph.add_edge(4, 5, 2) == 2);
    assert(graph.add_edge(5, 6, 2) == 3);
    assert(graph.add_edge(6, 7, 2) == 4);
    assert(graph.add_edge(7, 8, -1) == 5);
    assert(!graph.find_path_bfs(4, 8, &path));
    assert(graph.find_path_bfs(4, 7, &path));
    assert(path.size() == 3);

    // if there are multiple paths, use one with fewest edges.
    assert(graph.add_edge(4, 7, 1) == 6);
    assert(graph.find_path_bfs(4, 7, &path));
    assert(path.size() == 1);
    assert(path[0] == 6);
  }

  // Test max flow
  {
    // Trivial case.
    {
      Graph<> graph(2);
      assert(graph.add_edge(0, 1, 3) == 0);
      assert(graph.edge(0).weight == 3);
      assert(graph.find_max_flow(0, 1) == 3);
      assert(graph.edge(0).weight == 0);
    }
    // Simple sequence case.
    {
      Graph<> graph(5);
      assert(graph.add_edge(0, 1, 3) == 0);
      assert(graph.add_edge(1, 2, 5) == 1);
      assert(graph.edge(0).weight == 3);
      assert(graph.edge(1).weight == 5);
      assert(graph.find_max_flow(0, 2) == 3);
      assert(graph.edge(0).weight == 0);
      assert(graph.edge(1).weight == 2);
    }
    // Simple branching case.
    {
      Graph<> graph(5);
      assert(graph.add_edge(0, 1, 3) == 0);
      assert(graph.add_edge(1, 2, 5) == 1);
      assert(graph.add_edge(0, 2, 12) == 2);
      assert(graph.edge(0).weight == 3);
      assert(graph.edge(1).weight == 5);
      assert(graph.edge(2).weight == 12);
      assert(graph.find_max_flow(0, 2) == 15);
      assert(graph.edge(0).weight == 0);
      assert(graph.edge(1).weight == 2);
      assert(graph.edge(2).weight == 0);
    }
  }

  // Test find_min_cut
  {
    Graph<> graph(5);
    assert(graph.add_edge(0, 1, 7) == 0);
    graph.add_edge(1, 2, 77);
    graph.add_edge(2, 3, 777);
    std::vector<Graph<>::EdgeId> edge_ids;
    assert(graph.find_min_cut(0, 3, &edge_ids) == 7);
    assert(edge_ids.size() == 1);
    assert(edge_ids[0] == 0);
  }

  // Test custom Graph options.
  {
    struct GraphOpt {
      typedef int WeightType;
    };
    static_assert(
        std::is_same<Graph<GraphOpt>::Weight, int>::value,
        "test that 'int' 'WeightType' is passed on to Graph");
    static_assert(
        !std::is_same<Graph<GraphOpt>::Weight, double>::value,
        "double check that Weight is not double");
  }

  // Test find_all_topological_orderings
  {
    Graph<> graph(3);
    std::vector<std::vector<Graph<>::NodeId>> orderings;
    // Test free ordering.
    graph.find_all_topological_orderings(&orderings);
    assert(orderings.size() == 6);  // 3!
    // All of the topological orderings should be in lexicographic order.
    assert(orderings[0] == std::vector<Graph<>::NodeId>({0, 1, 2}));
    assert(orderings[1] == std::vector<Graph<>::NodeId>({0, 2, 1}));

    // Test with just one restriction.
    graph.add_edge(1, 2, 1);
    graph.find_all_topological_orderings(&orderings);
    assert(orderings.size() == 3);
    assert(orderings[0] == std::vector<Graph<>::NodeId>({0, 1, 2}));
    assert(orderings[1] == std::vector<Graph<>::NodeId>({1, 0, 2}));
    assert(orderings[2] == std::vector<Graph<>::NodeId>({1, 2, 0}));

    // Test with full constraints.
    graph.add_edge(0, 1, 1);
    graph.find_all_topological_orderings(&orderings);
    assert(orderings.size() == 1);
    assert(orderings[0] == std::vector<Graph<>::NodeId>({0, 1, 2}));

    // Ignore non-positive edges.
    graph.add_edge(1, 0, -1);
    graph.add_edge(2, 1, 0);
    graph.find_all_topological_orderings(&orderings);
    assert(orderings.size() == 1);
    assert(orderings[0] == std::vector<Graph<>::NodeId>({0, 1, 2}));

    // Too many constraints.
    graph.add_edge(1, 0, 1);
    graph.find_all_topological_orderings(&orderings);
    assert(orderings.size() == 0);
  }

  std::cout << "*** All graph sanity checks pass! ***" << std::endl;
}

int main() {
  graph_sanity_check();
}
