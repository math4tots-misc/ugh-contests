"""adjlist.py"""

def make_adjacency_list_from_edge_list(N, edges):
  """makes an adjacency list from a list of edges.
  Args
    N: number of nodes in the graph.
    edges: list of 3-tuples (node1, node2, weight) describing edges.

  Returns adj
    An adjacency list where
      adj[x] is a list of 3-tuples (e, y, r) where
        e is the id of the edge (index in edges),
        y is the id of the node that this edge connects,
        r is the weight of this edge.
  """
  adjacency_list = [[] for _ in range(N)]
  for e, (x, y, r) in enumerate(edges):
    adjacency_list[x].append((e, y, r))
    adjacency_list[y].append((e, x, r))
  return adjacency_list

if __name__ == '__main__':

  al = make_adjacency_list_from_edge_list(3, [
      (1, 2, 5),
      (2, 0, 7),
  ])

  assert al == [
      [(1, 2, 7)],
      [(0, 2, 5)],
      [(0, 1, 5), (1, 0, 7)],
  ], al

  print('pass')
