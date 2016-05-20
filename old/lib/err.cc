#include <stdexcept>

#include <iostream>

class Err: public std::runtime_error {
public:
  Err(const std::string& what_arg): std::runtime_error(what_arg) {}
};

using namespace std;

int main() {
  cout << "pass (no tests)" << endl;
}
