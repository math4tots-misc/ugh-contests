try: input = raw_input
except NameError: pass

def prim(N, edges, S=0):
  """
  Args
    N: Number of nodes in graph. Nodes are numbered 0..N-1.
    edges: iterable of 3-tuples, (node1, node2, weight).
    S: Node to start expanding from. Defaults to 0.

  Returns
    Total weight cost of MST.
  """
  adjacency_matrix = [[None for n2 in range(N)] for n1 in range(N)]
  for x, y, r in edges:
    adjacency_matrix[x][y] = r
    adjacency_matrix[y][x] = r

  # nodes_in_tree contains all the nodes that we have added to the tree
  # so far.
  nodes_in_tree = set()

  # cost_to_add[n], is the cost of an edge we could pay to add node n
  # to the current tree.
  used_edge = [None for n in range(N)]
  used_edge[S] = 0  # We start from 'S', so no need to pay for it.

  while len(nodes_in_tree) < N:

    min_cost = None
    min_cost_node = None
    for m in range(N):
      if (m not in nodes_in_tree and
          used_edge[m] is not None and
          (min_cost is None or
           used_edge[m] < min_cost)):
        min_cost = used_edge[m]
        min_cost_node = m

    if min_cost_node is None:  # disconnected graph.
      raise Exception('disconnected graph')

    nodes_in_tree.add(min_cost_node)

    for n in range(N):
      if (n not in nodes_in_tree and
          adjacency_matrix[n][min_cost_node] is not None and
          (used_edge[n] is None or
           adjacency_matrix[n][min_cost_node] < used_edge[n])):
        used_edge[n] = adjacency_matrix[n][min_cost_node]

  return sum(r for r in used_edge if r is not None)


N, M = map(int, input().split())

weight_of = [[None for to_ in range(N)] for from_ in range(N)]

edges = []
for _ in range(M):
  x, y, r = map(int, input().split())
  edges.append((x-1, y-1, r))

S = int(input())-1

print(prim(N, edges, S))

