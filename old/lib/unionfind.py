"""unionfind.py"""

class UnionFind(object):

  def __init__(self, N):
    self._parent = list(range(N))
    self._size = [1] * N

  def find(self, i):
    if self._parent[i] != i:
      self._parent[i] = self.find(self._parent[i])
    return self._parent[i]

  def join(self, a, b):
    a = self.find(a)
    b = self.find(b)
    if a == b:
      return b
    if self._size[b] < self._size[a]:
      a, b = b, a

    self._size[b] += self._size[a]
    self._parent[a] = b
    return b

  def connected(self, a, b):
    return self.find(a) == self.find(b)

  def sizeof(self, i):
    return self._size[self.find(i)]

if __name__ == '__main__':
  uf = UnionFind(10)

  for i in range(10):
    assert uf.sizeof(i) == 1, uf.sizeof(i)
    for j in range(i+1, 10):
      assert not uf.connected(i, j)

  for i in range(1, 10):
    uf.join(1, i)

  assert uf.sizeof(0) == 1, uf.sizeof(0)
  for i in range(1, 10):
    assert uf.sizeof(i) == 9, (i, uf.sizeof(i))
    assert uf.find(i) == uf.find(1), uf.find(i)
    assert uf.connected(1, i)

  print('pass')
