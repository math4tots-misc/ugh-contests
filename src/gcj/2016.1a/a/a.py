from collections import deque

try: input = raw_input
except NameError: pass

def solve(S):
  d = deque(S[0])
  for c in S[1:]:
    if c >= d[0]:
      d.appendleft(c)
    else:
      d.append(c)
  return ''.join(d)

for i in range(1, 1+int(input())):
  print 'Case #%r: %s' % (i, solve(input()))


