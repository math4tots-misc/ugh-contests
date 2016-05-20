"""kruskal.py"""
#x unionfind.py

def kruskal(N, edges):
  """Kruskal's algorithm for finding the minimal spanning forest (MSF).

  Args
    N: number of nodes in the graph. Nodes are numbered 0..N-1.
    edges: list of 3-tuples (node1, node2, weight) describing edges.

  Returns mst
    The MSF described by a set of edge indices (up to length N-1).
  """

  enum_edges = [(e, x, y, r) for e, (x, y, r) in enumerate(edges)]
  enum_edges.sort(key=lambda edge: edge[-1])
  uf = UnionFind(N)
  mst = []

  for e, x, y, r in enum_edges:
    if not uf.connected(x, y):
      uf.join(x, y)
      mst.append(e)

  return set(mst)

if __name__ == '__main__':
  while 1:
    # Tests modified from prim
    edges = [
        (0, 1, 4),
        (0, 2, 100),
        (1, 2, 55),
    ]
    mst = kruskal(3, edges)
    assert mst == {0, 2}, mst

    edges = [
        (0, 1, 4),
        (1, 2, 55),
        (0, 2, 100),
    ]
    assert kruskal(3, edges) == {0, 1}

    edges = [
        (0, 1, 4),
        (1, 2, 55),
        (0, 2, 100),
        (3, 4, 1),  # not in connected component of '0'
    ]
    mst = kruskal(5, edges)
    assert mst == {0, 1, 3}, mst

    # Verify that 'added' is used
    edges = [
        (0, 1, 1),
        (1, 2, 2),
        (2, 3, 1),
    ]
    mst = kruskal(4, edges)
    assert mst == {0, 1, 2}, mst

    break

  print('pass')
