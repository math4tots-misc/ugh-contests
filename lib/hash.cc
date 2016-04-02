#include <iostream>

// This is what boost does.
void hash_combine(std::size_t& seed, const int& v) {
  std::hash<int> hasher;
  seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

using namespace std;
int main() {
  cout << "There is no test for hash.cc" << endl;
}
