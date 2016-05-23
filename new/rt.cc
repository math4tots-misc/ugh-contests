// Autogenerated from <xc> programming language.
// Needs C++14 features
////// BEGIN prelude
#include <iostream>
#include <functional>
#include <type_traits>
#include <initializer_list>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <tuple>

#define vvnil nullptr
#define vvself this
#define vvtrue true
#define vvfalse false

class CCObject;

template <class T>
class P {
public:
  static_assert(std::is_base_of<CCObject, T>::value, "Must subclass Object");
  P(): ptr(nullptr) {}
  P(T* p): ptr(p) { incr(); }
  P(const P& p): P(p.ptr) {}
  ~P() { decr(); }
  P& operator=(const P& p) { p.incr(); decr(); ptr = p.ptr; return *this; }
  T* operator->() const { return ptr; }
  template <class K> auto operator==(K k) const { return ptr->mmmmeq(k); }
  template <class K> auto operator!=(K k) const { return ptr->mmmmne(k); }
  template <class K> auto operator<(K k) const { return ptr->mmmmlt(k); }
  template <class K> auto operator+(K k) const { return ptr->mmmmadd(k); }
  template <class K> auto operator-(K k) const { return ptr->mmmmsub(k); }
  template <class K> auto operator*(K k) const { return ptr->mmmmmul(k); }
  template <class K> auto operator/(K k) const { return ptr->mmmmdiv(k); }
  template <class K> auto operator%(K k) const { return ptr->mmmmmod(k); }
  auto operator++() const { return ptr->mmmmincr(); }
  auto operator*() const { return ptr->mmmmderef(); }
  auto begin() const { return ptr->mmmmbegin(); }
  auto end() const { return ptr->mmmmend(); }
private:
  void incr() const { if (ptr) { ptr->increment(); } }
  void decr() const { if (ptr) { ptr->decrement(); } }
  T* ptr;
};

class CCObject {
public:
  CCObject(): refcnt(0) {}
  virtual ~CCObject() {}
private:
  int refcnt;
  void increment() { refcnt++; }
  void decrement() { refcnt--; if (!refcnt) { delete this; } }
  template <class T> friend class P;
};

typedef void PPVoid;
typedef bool PPBool;
typedef char PPChar;
typedef long PPInt;
typedef double PPFloat;
template <class... T> using PPTuple = std::tuple<T...>;
template <class R, class... A> using PPFunction = std::function<R(A...)>;

template <class... T>
class CCTupleObject final: public CCObject {
public:
  CCTupleObject(const PPTuple<T...>& v): value(v) {}
  CCTupleObject(const CCTupleObject<T...>& v): value(v.value) {}
private:
  PPTuple<T...> value;
};
template <class... T> using PPTupleObject = CCTupleObject<T...>;

class PPAny final {
public:
  static constexpr int POINTER = 0;
  static constexpr int BOOL = 1;
  static constexpr int CHAR = 2;
  static constexpr int INT = 3;
  static constexpr int FLOAT = 4;
  PPAny(): type(POINTER) {}
  template <class T> PPAny(const T& value) { assign(value); }
  template <class T>
  PPAny& operator=(const T& value) {
    assign(value);
    return *this;
  }

private:

  template <class T>
  void assign(P<T> p) {
    type = POINTER;
    pointer = p;
  }

  template <class... T>
  void assign(PPTuple<T...> t) {
    type = POINTER;
    pointer = new CCTupleObject<T...>(t);
  }

  void assign(PPBool c) {
    type = BOOL;
    boolean = c;
    pointer = nullptr;
  }

  void assign(PPChar c) {
    type = CHAR;
    character = c;
    pointer = nullptr;
  }

  void assign(PPInt c) {
    type = INT;
    integer = c;
    pointer = nullptr;
  }

  void assign(PPFloat c) {
    type = FLOAT;
    floating = c;
    pointer = nullptr;
  }

  int type;
  P<CCObject> pointer;
  union {
    PPBool boolean;
    PPChar character;
    PPInt integer;
    PPFloat floating;
  };
};

class CCString;
typedef P<CCString> PPString;
class CCString final: public CCObject {
public:
  CCString(const std::string& v): s(v) {}
  const std::string str() const { return s; }
  PPInt mmsize() const { return s.size(); }
  PPChar mmmmdiv(PPInt i) const { return s[i]; }
  auto mmmmbegin() { return s.begin(); }
  auto mmmmend() { return s.end(); }
private:
  const std::string s;
};

template <class T> class CCVector;
template <class T> using PPVector = P<CCVector<T>>;

template <class T>
class CCVector final: public CCObject {
public:
  CCVector(std::initializer_list<T> args): v(args) {}
  PPInt mmsize() const { return v.size(); }
  T mmmmdiv(PPInt i) const { return v[i]; }
  void mmmmsetitem(PPInt i, T v) { v[i] = v; }
  void mmpush(T t) { v.push_back(t); }
  auto mmmmbegin() { return v.begin(); }
  auto mmmmend() { return v.end(); }
  PPString mmmmstr() {
    std::stringstream ss;
    ss << "$[";
    bool first = true;
    for (auto i: v) {
      if (!first) {
        ss << ", ";
      }
      ss << i;
      first = false;
    }
    ss << "]";
    return new CCString(ss.str());
  }
private:
  std::vector<T> v;
};

// make Vector
template <class T>
PPVector<T> V(std::initializer_list<T> args) {
  return new CCVector<T>(args);
}

// Tuple out stream
template <class Tuple, int I>
struct TupleWriteHelper {
  static void write(std::ostream& out, const Tuple& t) {
    out << std::get<std::tuple_size<Tuple>::value-I>(t) << ", ";
    TupleWriteHelper<Tuple, I-1>::write(out, t);
  }
};

template <class Tuple>
struct TupleWriteHelper<Tuple, 1> {
  static void write(std::ostream& out, const Tuple& t) {
    out << std::get<std::tuple_size<Tuple>::value-1>(t);
  }
};

template <class... A>
std::ostream& operator<<(std::ostream& out, const PPTuple<A...>& t) {
  out << "T[";
  TupleWriteHelper<
      PPTuple<A...>,
      std::tuple_size<PPTuple<A...>>::value
  >::write(out, t);
  return out << "]";
}

std::ostream& operator<<(std::ostream& out, const PPString& s) {
  return out << s->str();
}

template <class T>
std::ostream& operator<<(std::ostream& out, const P<T>& t) {
  return out << t->mmmmstr();
}

// make Tuple
template <class... A>
PPTuple<A...> vvT(A... args) {
  return PPTuple<A...>(args...);
}

// print
template <class T>
void vvprint(const T& t) {
  std::cout << t << std::endl;
}

PPVector<PPString> vvARGS(new CCVector<PPString>({}));

void vvmain();

int main(int argc, char **argv) {
  for (int i = 0; i < argc; i++) {
    vvARGS->mmpush(new CCString(argv[i]));
  }
  vvmain();
}

////// END prelude
using namespace std;

int main() {
  auto v = V({1, 2, 3});
  cout << v->mmsize() << endl;
}
