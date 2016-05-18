#include <stdexcept>

class Err: public std::runtime_error {
public:
  Err(const std::string& what_arg): std::runtime_error(what_arg) {}
};

using namespace std;

#include <iostream>

int main() {
  cout << "pass (no tests for 'err')" << endl;
}
