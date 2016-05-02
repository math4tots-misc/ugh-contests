try: input = raw_input
except NameError: pass

def match(fmt, n):
  ns = str(n).zfill(len(fmt))
  for a, b in zip(fmt, ns):
    if a != '?' and a != b:
      return False
  return True

def solve(line):
  a, b = line.split()

  ba = bb = None

  ia = 0
  while len(str(ia)) <= len(a):
    ib = 0
    while len(str(ib)) <= len(b):
      if match(a, ia) and match(b, ib):
        if ba is None or abs(ia-ib) < abs(ba-bb):
          ba = ia
          bb = ib
      ib += 1
    ia += 1

  return '%s %s' % (str(ia).zfill(len(a)), str(ib).zfill(len(b)))

for i in range(1, 1+int(input())):
  print('Case #%d: %s' % (i, solve(input())))
