"""IO#$HPO@#$H#@LKJWHF:KSFJ 

Got it right with 3 seconds left, but didn't have enough time to submit...

(by 'got it right', I mean that it passes the 'practice' test cases)
"""

try: input = raw_input
except NameError: pass

def debug(*args, **kwargs):
  # print(*args, **kwargs)
  pass

def solve(N, bffs):
  debug('bffs = %r' % dict(enumerate(bffs,)))
  lines = dict()
  cycles = set()
  pairs = dict()

  for i in range(N):
    last = i
    j = bffs[i]
    visited = set([i])
    while j not in visited:
      visited.add(j)
      last = j
      j = bffs[last]

    if bffs[bffs[j]] == j:  # 2 cycle
      key = tuple(sorted((j, bffs[j])))
      if key not in lines:
        lines[key] = []
        pairs[key] = [[], []]
      if j == key[0]:
        pairs[key][0].append(visited)
      else:
        pairs[key][1].append(visited)
      lines[key].append(visited)
      debug('line = %r (%d)' % (repr(visited), len(visited)))
    elif j == i:  # cycle including i
      cycles.add(len(visited))
      debug('cycle = %r, (%d)' % (repr(visited), len(visited)))

  linecycles = set()
  for key, (aa, bb) in pairs.items():
    cand = set()
    for a in aa:
      for b in bb:
        lines[key].append([0] * len(a|b))

  # You can either include all the lines, or pick one cycle.
  ans = sum(max(len(choice) for choice in choices)
            for choices in lines.values())
  if linecycles:
    ans = max(ans, sum(linecycles))
  if cycles:
    ans = max(ans, max(cycles))

  return ans

for i in range(1, 1+int(input())):
  N = int(input())
  bffs = [int(x)-1 for x in input().split()]
  assert N == len(bffs), '%r, %r' % (N, len(bffs))
  print('Case #%d: %d' % (i, solve(N, bffs)))
