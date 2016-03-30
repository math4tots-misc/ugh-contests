/*
ID: math4to2
LANG: C++11
TASK: frameup
*/

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <deque>
#include <vector>
#include <unordered_map>

// NOTE: everything up to 'using namespace std' is from my personal lib.

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
    if (stage->size() == N) {
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

unordered_map<int, char> i2lm;
unordered_map<char, int> l2im;

int l2i(char letter) {
  if (l2im.find(letter) == l2im.end()) {
    int i = i2lm.size();
    i2lm[i] = letter;
    l2im[letter] = i;
  }

  return l2im[letter];
}

char i2l(int i) {
  return i2lm[i];
}

int main() {
  typedef Graph<> G;

  ifstream fin("frameup.in");
  ofstream fout("frameup.out");

  int H, W, N = 0;
  fin >> H >> W;

  string line;
  vector<string> p;
  for (int i = 0; i < H; i++) {
    fin >> line;
    p.push_back(line);
  }

  for (int h = 0; h < H; h++) {
    for (int w = 0; w < W; w++) {
      if (p[h][w] != '.') {
        int pN = 1 + l2i(p[h][w]);
        N = N > pN ? N : pN;
      }
    }
  }

  G graph(N);
  vector<int> left(N, -1);
  vector<int> right(N, -1);
  vector<int> up(N, -1);
  vector<int> down(N, -1);

  for (int h = 0; h < H; h++)
    for (int w = 0; w < W; w++)
      if (p[h][w] != '.' && up[l2i(p[h][w])] == -1)
        up[l2i(p[h][w])] = h;

  for (int h = H-1; h >= 0; h--)
    for (int w = 0; w < W; w++)
      if (p[h][w] != '.' && down[l2i(p[h][w])] == -1)
        down[l2i(p[h][w])] = h;

  for (int w = 0; w < W; w++)
    for (int h = 0; h < H; h++)
      if (p[h][w] != '.' && left[l2i(p[h][w])] == -1)
        left[l2i(p[h][w])] = w;

  for (int w = W-1; w >= 0; w--)
    for (int h = 0; h < H; h++)
      if (p[h][w] != '.' && right[l2i(p[h][w])] == -1)
        right[l2i(p[h][w])] = w;

  for (int n = 0; n < N; n++) {
    int w, h;

    h = up[n];
    for (w = left[n]; w <= right[n]; w++)
      if (p[h][w] != '.' && l2i(p[h][w]) != n)
        graph.set_edge(n, l2i(p[h][w]), 1);

    h = down[n];
    for (w = left[n]; w <= right[n]; w++)
      if (p[h][w] != '.' && l2i(p[h][w]) != n)
        graph.set_edge(n, l2i(p[h][w]), 1);

    w = left[n];
    for (h = up[n]; h <= down[n]; h++)
      if (p[h][w] != '.' && l2i(p[h][w]) != n)
        graph.set_edge(n, l2i(p[h][w]), 1);

    w = right[n];
    for (h = up[n]; h <= down[n]; h++)
      if (p[h][w] != '.' && l2i(p[h][w]) != n)
        graph.set_edge(n, l2i(p[h][w]), 1);
  }

  vector<vector<G::NodeId>> orderings;
  graph.find_all_topological_orderings(&orderings);

  vector<vector<char>> ans;
  for (auto& ord: orderings) {
    vector<char> cc;
    for (G::NodeId i: ord) {
      cc.push_back(i2l(i));
    }
    ans.push_back(cc);
  }

  sort(ans.begin(), ans.end());

  for (auto& ord: ans) {
    for (char i: ord) {
      fout << i;
    }
    fout << endl;
  }
}
