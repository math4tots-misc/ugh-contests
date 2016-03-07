"""vertical-rooks.py

The spaces 'behind' a given rook don't matter since on the next turn
the opponent can nullify that effect.

This problem maps naturally to nim.

"""

import sys

nums = (int(line) for line in sys.stdin.read().splitlines())

for _ in range(next(nums)):
  N = next(nums)
  p1 = [next(nums) for _ in range(N)]
  nimsum = 0
  for a in p1:
    nimsum ^= abs(a-next(nums))-1
  if nimsum:
    print('player-2')
  else:
    print('player-1')

