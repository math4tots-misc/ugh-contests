"""prim.py"""
#x heap.py
#x adjlist.py

def prim(N, edges, S=0):
  """Prim's algorithm for finding the minimal spanning tree (MST)

  Args
    N: number of nodes in the graph. Nodes are numbered 0..N-1.
    edges: list of 3-tuples, (node1, node2, weight) describing edges.
    S: The source node to start expanding from. Defaults to zero.

  Returns mst
    The MST described by a set of edge indices (up to length N-1).

  Note that in this implementation, we do not find a forest, but a single
  connected tree. So if there are disconnected components, we only find
  the MST of the connected component containing node S.
  """
  adjacency_list = make_adjacency_list_from_edge_list(N, edges)
  added = set()
  mst = [None for _ in range(N)]
  queue = Heap()
  queue[S] = 0

  while queue:
    n = queue.pop()
    added.add(n)
    for e, m, r in adjacency_list[n]:
      if m not in added and (mst[m] is None or r < edges[mst[m]][-1]):
        mst[m] = e
        queue[m] = r

  return {e for e in mst if e is not None}

if __name__ == '__main__':

  edges = [
      (0, 1, 4),
      (0, 2, 100),
      (1, 2, 55),
  ]
  assert prim(3, edges) == {0, 2}

  edges = [
      (0, 1, 4),
      (1, 2, 55),
      (0, 2, 100),
  ]
  assert prim(3, edges) == {0, 1}

  edges = [
      (0, 1, 4),
      (1, 2, 55),
      (0, 2, 100),
      (3, 4, 1),  # not in connected component of '0'
  ]
  assert prim(5, edges, 0) == {0, 1}

  edges = [
      (0, 1, 4),
      (1, 2, 55),
      (0, 2, 100),
      (3, 4, 1),  # not in connected component of '0'
  ]
  assert prim(5, edges, 3) == {3}

  # Verify that 'added' is used
  edges = [
      (0, 1, 1),
      (1, 2, 2),
      (2, 3, 1),
  ]
  mst = prim(4, edges, 0)
  assert mst == {0, 1, 2}, mst

  print('pass')
