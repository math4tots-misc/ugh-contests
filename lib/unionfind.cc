#include <vector>

struct UnionFind {

  typedef int Index;
  typedef Index Size;

  UnionFind(Index size): parent_of(size), size_of(size, 1), ngroups(size) {
    for (int i = 0; i < size; i++) {
      parent_of[i] = i;
    }
  }

  Index find(Index i) {
    if (parent_of[i] != i) {
      parent_of[i] = find(parent_of[i]);
    }
    return parent_of[i];
  }

  void join(Index raw_a, Index raw_b) {
    const Index a = find(raw_a);
    const Index b = find(raw_b);
    if (a == b) {
      return;
    }
    ngroups--;
    const Size ra = size_of[a];
    const Size rb = size_of[b];
    if (ra < rb) {
      parent_of[a] = b;
      size_of[b] += size_of[a];
    } else {
      parent_of[b] = a;
      size_of[a] += size_of[b];
    }
  }

  Index make_set() {
    Index i = parent_of.size();
    parent_of.push_back(i);
    size_of.push_back(1);
    return i;
  }

  Size size(Index i) {
    return size_of[find(i)];
  }

  Size number_of_nodes() const {
    return parent_of.size();
  }

  Size number_of_groups() const {
    return ngroups;
  }

private:
  std::vector<Index> parent_of;
  std::vector<Size> size_of;
  Size ngroups;
};

using namespace std;

#include <iostream>
#include <assert.h>

int main() {
  {
    UnionFind uf(10);
    assert(uf.find(1) != uf.find(2));
    assert(uf.find(1) == uf.find(1));
    assert(uf.size(1) == 1);
    assert(uf.number_of_nodes() == 10);
    assert(uf.number_of_groups() == 10);
    uf.join(1, 2);
    assert(uf.find(1) == uf.find(2));
    assert(uf.size(1) == 2);
    assert(uf.number_of_nodes() == 10);
    assert(uf.number_of_groups() == 9);
    uf.join(1, 2);
    assert(uf.find(1) == uf.find(2));
    assert(uf.size(1) == 2);
    assert(uf.number_of_nodes() == 10);
    assert(uf.number_of_groups() == 9);
  }
  cout << "pass" << endl;
}
