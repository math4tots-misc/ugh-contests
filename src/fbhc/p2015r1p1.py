"""2015 Round 1, problem 1: Homework"""
#x primes.py

UPPER_BOUND = 10 ** 7 + 1

PRIMES = primes(UPPER_BOUND)

PRIMACITY = [0] * UPPER_BOUND

for prime in PRIMES:
  for x in range(prime, UPPER_BOUND, prime):
    PRIMACITY[x] += 1

def solve(A, B, K):
  return sum(1 for x in range(A, B+1) if PRIMACITY[x] == K)

for t in range(1, int(input())+1):
  A, B, K = map(int, input().split())
  print('Case #%d: %d' % (t, solve(A, B, K)))
