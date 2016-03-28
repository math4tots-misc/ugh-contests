/*
ID: math4to2
LANG: C++11
TASK: wormhole
*/

/*

Test with:
g++ --std=c++11 -Wall -Werror -Wpedantic wormhole.cc && \
    cat wormhole.in | ./a.out

N is only 12, so at most there are 11 * 9 * 7 * 5 * 3 < 10^5 configs.
So brute force is appropriate here.
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <unordered_set>
#include <algorithm>

using namespace std;

struct {
  bool operator()(const tuple<int, int>& a, const tuple<int, int>& b) {
    return get<1>(a) < get<1>(b) ||
        (get<1>(a) == get<1>(b) && get<0>(a) < get<0>(b));
  }
} group_by_y_then_sort_by_x_comparator;

struct Recurser {
  const int N;
  const vector<int>& paths;
  vector<int> pairs;
  vector<bool> used;

  Recurser(int N, const vector<int>& paths):
      N(N), paths(paths), pairs(N, -1), used(N, false) {}

  // 'paths[i]' should be the index of the wormhole that is to the right of
  //   wormhole 'i'. If there is no wormhole to the right, it should be -1.
  // 'pairs[i]' should be the index of the womrhole that wormhole 'i' is
  //   paired up with.
  bool has_cycle() {
    for (int start = 0; start < N; start++) {
      int x = paths[pairs[start]];

      while (x > start)
        x = paths[pairs[x]];

      if (x == start)
        return true;
    }
    return false;
  }

  int recurse() {
    int left;
    for (left = 0; left < N; left++) {
      if (!used[left])
        break;
    }

    if (left == N)  // If all of them are used, just check for cycle.
      return has_cycle();

    int total = 0;
    used[left] = true;
    for (int right = left+1; right < N; right++) {
      if (!used[right]) {
        used[right] = true;
        pairs[left] = right;
        pairs[right] = left;
        total += recurse();
        used[right] = false;
      }
    }
    used[left] = false;
    return total;
  }
};

int main() {
  ifstream fin("wormhole.in");
  ofstream fout("wormhole.out");

  int N;
  vector<tuple<int, int>> pts;
  fin >> N;
  for (int i = 0; i < N; i++) {
    tuple<int, int> pair;
    fin >> get<0>(pair) >> get<1>(pair);
    pts.push_back(pair);
  }

  sort(pts.begin(), pts.end(), group_by_y_then_sort_by_x_comparator);

  vector<int> paths(N, -1);
  for (int i = 0; i < N-1; i++) {
    if (get<1>(pts[i]) == get<1>(pts[i+1]))
      paths[i] = i+1;
  }

  fout << Recurser(N, paths).recurse() << endl;
}
