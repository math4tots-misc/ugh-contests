#include <stdio.h>

#define MAX_N 400
#define MAX_M ((400 * 399) / 2)
#define MAX_R 350
#define MAX_DIST (MAX_M * MAX_R + 1)

int N, M, Q;
int x[MAX_M];
int y[MAX_M];
int r[MAX_M];
int dist[MAX_N][MAX_N];

int main() {
  scanf("%d%d", &N, &M);
  for (int i = 0; i < M; i++) {
    scanf("%d%d%d", x+i, y+i, r+i);
  }
  for (int a = 0; a < N; a++) {
    for (int b = 0; b < N; b++) {
      dist[a][b] = MAX_DIST;
    }
    dist[a][a] = 0;
  }
  for (int i = 0; i < M; i++) {
    dist[x[i]-1][y[i]-1] = r[i];
  }
  for (int k = 0; k < N; k++) {
    for (int a = 0; a < N; a++) {
      for (int b = 0; b < N; b++) {
        int estimate = dist[a][k] + dist[k][b];
        if (estimate < dist[a][b]) {
          dist[a][b] = estimate;
        }
      }
    }
  }
  scanf("%d", &Q);
  for (int i = 0; i < Q; i++) {
    int a, b;
    scanf("%d%d", &a, &b);
    a--;
    b--;
    int result = (dist[a][b] == MAX_DIST) ? -1 : dist[a][b];
    printf("%d\n", result);
  }
}



