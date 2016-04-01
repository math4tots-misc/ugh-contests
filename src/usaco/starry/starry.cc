/*
ID: math4to2
LANG: C++11
TASK: starry
*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>
#include <assert.h>

using namespace std;

struct RawCluster {

  RawCluster(const vector<string>& data) {
    height = data.size();
    width = data[0].size();
    for (const auto& line: data) {
      buffer += line;
    }
  }

  bool operator==(const RawCluster& c) const {
    return width == c.width && height == c.height && buffer == c.buffer;
  }

  bool operator!=(const RawCluster& c) const {
    return width != c.width || height != c.height || buffer != c.buffer;
  }

  void normalize() {
    int w = width, h = height;
    string b = buffer;
    for (int i = 0; i < 4; i++) {
      rotate90();
      for (int j = 0; j < 2; j++) {
        if (width < w || (width == w && buffer < b)) {
          b = buffer;
          w = width;
          h = height;
        }
        reflect();
      }
    }
    width = w;
    height = h;
    buffer = b;
  }

  void rotate90() {  // rotates 90 degrees clockwise.
    int w = height, h = width;
    string b(buffer.size(), '0');

    for (int iw = 0; iw < width; iw++) {
      for (int ih = 0; ih < height; ih++) {
        b[indexof(w, h, w-1-ih, iw)] = buffer[indexof(width, height, iw, ih)];
      }
    }

    width = w;
    height = h;
    buffer = b;
  }

  void reflect() {
    for (int h = 0; h < height; h++) {
      for (int w = 0; 2*w < width; w++) {
        char c = buffer[indexof(width, height, w, h)];
        buffer[indexof(width, height, w, h)] =
            buffer[indexof(width, height, width-1-w, h)];
        buffer[indexof(width, height, width-1-w, h)] = c;
      }
    }
  }

  size_t compute_hash() const {
    hash<string> hasher;
    size_t hash_value = hasher(buffer);
    hash_combine(hash_value, width);
    hash_combine(hash_value, height);
    return hash_value;
  }

  string to_string() const {
    string display;
    int i = 0;
    for (int h = 0; h < height; h++) {
      for (int w = 0; w < width; w++, i++) {
        display += buffer[i];
      }
      display += "\n";
    }
    return display;
  }

private:

  // This is basically what boost does
  static void hash_combine(std::size_t& seed, const int& v) {
    std::hash<int> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
  }

  static int indexof(int width, int height, int w, int h) {
    return h * width + w;
  }

  int width, height;
  string buffer;
};

struct Cluster {
  Cluster(const vector<string>& data): rc(data) {
    rc.normalize();
    saved_hash = rc.compute_hash();
  }

  bool operator==(const Cluster& c) const {
    return rc == c.rc;
  }

  size_t get_hash() const {
    return saved_hash;
  }

  string to_string() const {
    return rc.to_string();
  }

private:
  RawCluster rc;
  size_t saved_hash;
};

namespace std {

  template<>
  struct hash<Cluster> {
    size_t operator()(const Cluster& c) const {
      return c.get_hash();
    }
  };
}

void floodfill(vector<string>& b, int h, int w, char oldc, char c,
               int* minw, int* minh, int* maxw, int* maxh) {
  int H = b.size();
  int W = b[0].size();
  if (h < 0 || w < 0 || h >= H || w >= W || b[h][w] != oldc)
    return;

  b[h][w] = c;

  if (h > *maxh)
    *maxh = h;
  if (h < *minh)
    *minh = h;
  if (w > *maxw)
    *maxw = w;
  if (w < *minw)
    *minw = w;

  floodfill(b, h+1, w  , oldc, c, minw, minh, maxw, maxh);
  floodfill(b, h+1, w-1, oldc, c, minw, minh, maxw, maxh);
  floodfill(b, h+1, w+1, oldc, c, minw, minh, maxw, maxh);
  floodfill(b, h-1, w  , oldc, c, minw, minh, maxw, maxh);
  floodfill(b, h-1, w-1, oldc, c, minw, minh, maxw, maxh);
  floodfill(b, h-1, w+1, oldc, c, minw, minh, maxw, maxh);
  floodfill(b, h  , w-1, oldc, c, minw, minh, maxw, maxh);
  floodfill(b, h  , w+1, oldc, c, minw, minh, maxw, maxh);
}

void cluster_test() {
  // rotate
  {
    RawCluster a(std::vector<string>({
      "1000",
      "0000",
    }));
    RawCluster c = a;
    RawCluster b(std::vector<string>({
      "01",
      "00",
      "00",
      "00",
    }));
    assert(a != b);
    assert(a == c);
    a.rotate90();
    assert(a == b);
    assert(a != c);
    a.rotate90();
    a.rotate90();
    a.rotate90();
    assert(a == c);
    assert(a.compute_hash() == c.compute_hash());
    RawCluster d(std::vector<string>({
      "10",
      "00",
    }));
    RawCluster e(std::vector<string>({
      "01",
      "00",
    }));
    d.reflect();
    assert(d == e);
  }
  // floodfill
  {
    vector<string> bb({
      "0000",
      "1110",
    });
    int a = 10, b = 10, c = -1, d = -1;
    floodfill(bb, 0, 0, '0', '2', &a, &b, &c, &d);
    assert(a == 0);
    assert(b == 0);
    assert(c == 3);
    assert(d == 1);
    assert(bb == vector<string>({
      "2222",
      "1112",
    }));
  }
  cout << "All cluster tests pass!" << endl;
}

int main() {
  // cluster_test();
  int W, H;
  ifstream fin("starry.in");
  ofstream fout("starry.out");
  fin >> W >> H;
  vector<string> b(H, "");
  for (int i = 0; i < H; i++)
    fin >> b[i];

  char nextch = 'a';
  unordered_map<Cluster, char> seen;
  for (int h = 0; h < H; h++) {
    for (int w = 0; w < W; w++) {
      if (b[h][w] == '1') {
        int min_w = W, min_h = H, max_w = -1, max_h = -1;
        floodfill(b, h, w, '1', '2', &min_w, &min_h, &max_w, &max_h);
        vector<string> data(max_h-min_h+1, string(max_w-min_w+1, '0'));
        for (int ih = min_h; ih <= max_h; ih++)
          for (int iw = min_w; iw <= max_w; iw++)
            data[ih-min_h][iw-min_w] = b[ih][iw] == '2' ? '1' : '0';
        Cluster c(data);
        auto iter = seen.find(c);
        char ch;
        if (iter == seen.end()) {
          seen[c] = ch = nextch++;
          cout << ch << endl << c.to_string() << endl;
        } else
          ch = iter->second;
        floodfill(b, h, w, '2', ch, &min_w, &min_h, &max_w, &max_h);
      }
    }
  }
  for (auto& line: b) {
    fout << line << endl;
  }
}
