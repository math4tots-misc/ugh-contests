#x prim.py

try: input = raw_input
except NameError: pass

N, M = map(int, input().split())

weight_of = [[None for to_ in range(N)] for from_ in range(N)]

edges = []
for _ in range(M):
  x, y, r = map(int, input().split())
  edges.append((x-1, y-1, r))

S = int(input())-1

print(sum(edges[e][-1] for e in prim(N, edges, S)))

