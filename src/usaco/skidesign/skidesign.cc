/*
ID: math4to2
LANG: C++11
TASK: skidesign
*/

/*
g++ --std=c++11 -Wall -Werror skidesign.cc && ./a.out

1000 * 100 == 10^5 -- brute force is basically ok here.
*/

#include <fstream>
#include <vector>
using namespace std;

int main() {
  ifstream fin("skidesign.in");
  ofstream fout("skidesign.out");

  int N;
  fin >> N;
  vector<int> hs(N, -1);
  for (int i = 0; i < N; i++)
    fin >> hs[i];

  int best = 1000 * 100 * 100;

  for (int lower = 0; lower + 17 <= 100; lower++) {
    int upper = lower + 17;
    int cost = 0;

    for (int h: hs) {
      if (h < lower) {
        cost += (lower - h) * (lower - h);
      } else if (h > upper) {
        cost += (h - upper) * (h - upper);
      }
    }

    if (cost < best)
      best = cost;
  }

  fout << best << endl;
}
