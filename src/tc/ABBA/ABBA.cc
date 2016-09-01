/*
Let's say that the initial string is 'C'.

Then after each operation, we must have a string of the form

  (A*B){n}C(BA*){n,n+1}

for some non-negative integer n.

To solve the problem, we just check that 'target' matches this pattern.
*/
#include <string>
#include <algorithm>
using namespace std;

struct ABBA {
  string canObtain(string init, string target) {
    if (target.size() < init.size()) {
      return "Impossible";
    }
    int nbinit = count(init.begin(), init.end(), 'B');
    int nbtarg = count(target.begin(), target.end(), 'B');
    if (nbtarg < nbinit) {
      return "Impossible";
    }
    int nbdiff = nbtarg - nbinit;
    auto tbegin = target.begin();
    auto tend = target.end();
    for (int nbrhs = nbdiff/2 + nbdiff%2; nbrhs;) {
      tend--;
      if (*tend == 'B') {
        nbrhs--;
      }
    }
    for (int nblhs = nbdiff/2; nblhs;) {
      if (*tbegin == 'B') {
        nblhs--;
      }
      tbegin++;
    }
    if (nbdiff%2) {
      reverse(tbegin, tend);
    }
    return equal(tbegin, tend, init.begin()) ? "Possible" : "Impossible";
  }
};
