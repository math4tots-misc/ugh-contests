/*
ID: math4to2
LANG: C++11
TASK: theme
*/

/*
My solution turns here is a lot less elegant than the official
solution...
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <functional>
#include <unordered_map>
#include <assert.h>
using namespace std;

// This is what boost does.
void hash_combine(std::size_t& seed, const int& v) {
  std::hash<int> hasher;
  seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

// Using 'map<vector<int>, int>' in the 'check' function
// below was causing a memory error.
// So I changed it to 'unordered_map<Slice, int>'
// Only 16MB is allowed, but rough estimate 5K*5K~25mil
// already exceeds 16mil. Not to mention vector/map overhead.
// So this is a little struct that represents enough
// information of a slice withhout actual copying.
struct Slice {
  const vector<int>* nsp;
  int i, len;
  size_t hashcode;
  Slice(const vector<int>* nsp, int i, int len):
      nsp(nsp), i(i), len(len), hashcode(0) {
    hash<int> hasher;
    for (int j = i; j < i+len; j++)
      hash_combine(hashcode, hasher((*nsp)[j] - (*nsp)[i]));
  }

  bool operator==(const Slice& slice) const {
    if (len != slice.len)
      return false;
    for (int a = i, b = slice.i; a < i+len; a++, b++) {
      if ((*nsp)[a]-(*nsp)[i] !=
          (*(slice.nsp))[b]-(*(slice.nsp))[slice.i])
        return false;
    }
    return true;
  }

  bool operator!=(const Slice& slice) const {
    return !(*this == slice);
  }
};

namespace std {
  template<>
  struct hash<Slice> {
    size_t operator()(const Slice& slice) const {
      return slice.hashcode;
    }
  };
}

// checks whether there is a theme of at least length len.
// Requirements:
//   len > 0
//   ns.size() > len
bool check(const vector<int>& ns, int len) {
  unordered_map<Slice, int> starts;
  for (int i = 0, rem = ns.size();; i++, rem--) {
    if (i < len && rem < len)
      continue;

    Slice theme(&ns, i, len);

    auto kv = starts.find(theme);
    if (kv != starts.end() && kv->second + len <= i)
      return true;
    if (kv == starts.end())
      starts[theme] = i;

    if (i+len == ns.size())
      break;
  }
  return false;
}

// Use binary search to limit number of checks we have to do.
int solve(const vector<int>& ns) {
  int a = 4, b = ns.size();
  while (a+1 < b) {
    int c = (a+b)/2;
    if (check(ns, c))
      a = c;
    else
      b = c;
  }
  return a < 5 ? 0 : a;
}

void test() {
  vector<int> v({1, 2, 3, 4, 5, 1});
  Slice a(&v, 0, 2);
  Slice b(&v, 2, 2);
  Slice c(&v, 4, 2);
  assert(a == b);
  assert(a.hashcode == b.hashcode);
  assert(c != b);

  assert(!check(vector<int>({1}), 1));
  assert(check(vector<int>({1, 1}), 1));
  assert(check(vector<int>({1, 2}), 1));  // transposed
  assert(check(vector<int>({1, 2, 1, 2}), 2));
  assert(check(vector<int>({1, 2, 7, 8}), 2));  // transposed
  assert(!check(vector<int>({1, 2, 7, 4}), 2));

  assert(check(vector<int>({
    1, 2, 3, 4, 5,
    6, 7, 8, 9, 10}), 5));
  
  // Theme less than 5.
  assert(solve(vector<int>({
    1, 2, 3, 4, 5})) == 0);
  
  // Theme of exactly 5.
  assert(solve(vector<int>({
    1, 2, 3, 4, 5,
    6, 7, 8, 9, 10})) == 5);

  cout << "theme tests pass" << endl;
}

int main() {
  // test();

  int N;
  ifstream fin("theme.in");
  ofstream fout("theme.out");
  fin >> N;
  vector<int> ns(N);
  for (int i = 0; i < N; i++)
    fin >> ns[i];

  fout << solve(ns) << endl;
}
