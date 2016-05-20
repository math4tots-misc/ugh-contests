"""primes.py"""

def prime_sieve(N):
  """Returns a prime sive of 0..N-1, as a list of bool."""
  assert N >= 2, N
  sieve = [True] * N
  sieve[0] = sieve[1] = False
  for i in range(2, N):
    if sieve[i]:
      for j in range(i+i, N, i):
        sieve[j] = False
  return sieve

def primes(N):
  """Returns a list of primes up to but not including N."""
  return [n for n, is_prime in enumerate(prime_sieve(N)) if is_prime]

if __name__ == '__main__':
  pr = primes(11)
  assert pr == [2, 3, 5, 7], pr  # Note, does not include 11.
  pr = primes(12)
  assert pr == [2, 3, 5, 7, 11], pr
  print('pass')
