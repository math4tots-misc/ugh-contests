/*
ID: math4to2
LANG: C++11
TASK: milk6
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
using namespace std;
#define MAX_COST 3000000000
#define MAX_N 32
#define MAX_M 1000
#define COUNT_FACTOR 33
#define ORDER_FACTOR (1LL << 32)
typedef long long I;

I f(I& cost, I index) {
  // Make sure we prefer solutions with fewer number of edges (COUNT_FACTOR)
  // Also make sure that if there are multiple solutions with
  // same number of edges, we prefer solutions with edges with
  // smaller index (ORDER_FACTOR).

  // TODO: Consider using 128 bit integer for 'I'.
  return (cost * COUNT_FACTOR + 1) * ORDER_FACTOR + (1 << index);
}

I invf(I cost) {
  return cost/ORDER_FACTOR/COUNT_FACTOR;
}

int main() {
  ifstream fin("milk6.in");
  ofstream fout("milk6.out");
  I N, M, edges[MAX_M][3], E[MAX_N][MAX_N];
  int iN, iM;
  fin >> iN >> iM;
  N = iN;
  M = iM;
  for (I i = 0; i < M; i++) {
    int iA, iB, iC;
    fin >> iA >> iB >> iC;
    edges[i][0] = iA;
    edges[i][1] = iB;
    edges[i][2] = iC;
    edges[i][0]--;
    edges[i][1]--;
    edges[i][2] += f(edges[i][2], i);
    E[edges[i][0]][edges[i][1]] = edges[i][2];
  }

  I total_cost = 0;
  for (;;) {
    I P[MAX_N]; // backward pointer path.
    for (I i = 0; i < N; i++)
      P[i] = -1;
    P[0] = -2;

    deque<I> queue(1, 0);
    while (P[N-1] == -1 && !queue.empty()) {
      I i = queue.front();
      queue.pop_front();
      for (I j = 0; j < N; j++) {
        if (P[j] == -1 && E[i][j] > 0) {
          // cout << j << " <- " << i << " (" << E[i][j] << ") " << endl;
          P[j] = i;
          queue.push_back(j);
        }
      }
    }

    if (P[N-1] == -1)
      break;

    // cout << "Path: ";
    for (I i = N-1; i != 0; i = P[i]) {
      // cout << i << " (" << E[P[i]][i]<< ") ";
    }
    // cout << endl;

    I c = MAX_COST;
    for (I i = N-1; i != 0; i = P[i])
      c = c < E[P[i]][i] ? c : E[P[i]][i];
    // cout << "Cost: " << c << endl;
    for (I i = N-1; i != 0; i = P[i]) {
      E[P[i]][i] -= c;
      E[i][P[i]] += c;
    }
    total_cost += c;
  }

  bool mark[MAX_N];
  for (I i = 0; i < N; i++)
    mark[i] = false;
  mark[0] = true;
  vector<I> stack(1, 0);
  while (!stack.empty()) {
    I i = stack.back();
    stack.pop_back();
    for (I j = 0; j < N; j++) {
      if (!mark[j] && E[i][j] > 0) {
        mark[j] = true;
        stack.push_back(j);
      }
    }
  }

  vector<int> ans;
  for (I i = 0; i < M; i++)
    if (mark[edges[i][0]] && !mark[edges[i][1]])
      ans.push_back(i+1);

  fout << (int) invf(total_cost) << ' ' << ans.size() << endl;
  for (auto e: ans)
    fout << e << endl;

  return 0;
}

