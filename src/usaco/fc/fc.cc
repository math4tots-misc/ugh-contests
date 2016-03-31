/*
ID: math4to2
LANG: C++11
TASK: fc
*/

#include <iostream>
#include <iomanip>
#include <fstream>

#include <cmath>
#include <algorithm>
#include <vector>
#include <deque>
#include <tuple>

#define REAL_ERR_TOLERANCE 0.00001

typedef double Real;

const Real PI = std::acos((double) -1);

bool eq(Real a, Real b) {
  return std::abs(a-b) < REAL_ERR_TOLERANCE;
}

struct Point {
  Real x, y, z;
  Point(Real x, Real y, Real z): x(x), y(y), z(z) {}
  Point(Real x, Real y): Point(x, y, 0) {}
  Point operator+(Point p) const {
    return Point(x + p.x, y + p.y, z + p.z);
  }
  Point operator-(Point p) const {
    return Point(x - p.x, y - p.y, z - p.z);
  }
  Point operator*(Real r) const {
    return Point(x * r, y * r, z * r);
  }
  Point operator/(Real r) const {
    return Point(x / r, y / r, z / r);
  }
  Point& operator+=(Point p) {
    x += p.x;
    y += p.y;
    z += p.z;
    return *this;
  }
  Point& operator-=(Point p) {
    x -= p.x;
    y -= p.y;
    z -= p.z;
    return *this;
  }
  Point& operator/=(Real r) {
    x /= r;
    y /= r;
    z /= r;
    return *this;
  }
  bool operator<(Point p) const {
    return
        x < p.x || (x == p.x && y < p.y) ||
        (x == p.x && y == p.y && z < p.z);
  }
  bool operator==(Point p) const {
    return x == p.x && y == p.y && z == p.z;
  }
  bool operator!=(Point p) const {
    return x != p.x || y != p.y || z != p.z;
  }
  Real dot(Point p) const {
    return x*p.x + y*p.y + z*p.z;
  }
  Real size() const {
    return sqrt(dot(*this));
  }
  Real dist(Point p) const {
    return (*this - p).size();
  }
  Point cross(Point p) const {
    return Point(y*p.z - p.y*z, z*p.x - x*p.z, x*p.y - y*p.x);
  }

  // Let 'this' be 'B'.
  // Returns the angle in radians that you need to rotate
  // BA counter-clockwise to get to BC.
  // Returns a value in range [0, 2*PI].
  Real angle2d(Point a, Point c) const {
    Point ba = a - *this;
    Point bc = c - *this;
    Real dot = ba.dot(bc);
    Real det = Point(0, 0, 1).dot(ba.cross(bc));
    Real angle = std::atan2(det, dot);
    if (angle < 0)
      angle += 2*PI;
    return angle;
  }

  // Let 'this' be 'B'.
  // Computes the angle in radians that you need to rotate
  // BA to get to BC. Since this is for general 3D, there is no
  // set notion of a 'clockwise' direction.
  // It will instead return a value in range [0, PI].
  Real angle(Point a, Point c) const {
    Point ba = a - *this, bc = c - *this;
    return std::acos(ba.dot(bc)/ba.size()/bc.size());
  }
};

std::ostream& operator<<(std::ostream& out, Point p) {
  return out << "Point(" << p.x << ", " << p.y << ", " << p.z << ")";
}

struct Points {

  Points(const std::vector<Point>& points): points(points) {}

  void find_convex_hull(std::vector<Point>* out) const {
    out->clear();

    // Make sure all points we look at are unique.
    std::vector<Point> sorted_points = points;
    sort(sorted_points.begin(), sorted_points.end());
    std::vector<Point> pts;
    for (auto pt: sorted_points) {
      if (pts.empty() || pts.back() != pt)
        pts.push_back(pt);
    }

    // If there are fewer than 3 unique points, the solution is degenerate.
    if (pts.size() < 3) {
      for (Point pt: pts)
        out->push_back(pt);
      return;
    }

    auto mid = find_midpoint();
    auto rpt = mid + Point(1, 0);
    std::vector<std::tuple<Real, Real, Point>> ptts;
    for (auto pt: pts)
      ptts.push_back(std::make_tuple(mid.angle2d(rpt, pt), mid.dist(pt), pt));
    sort(ptts.begin(), ptts.end());

    std::deque<Point> hull;
    hull.push_back(std::get<2>(ptts[0]));
    hull.push_back(std::get<2>(ptts[1]));

    for (int i = 2; i < ptts.size(); i++) {
      Point c = std::get<2>(ptts[i]);
      Point b = hull[hull.size()-1];
      Point a = hull[hull.size()-2];

      while (b.angle2d(a, c) <= PI) {
        hull.pop_back();
        b = hull[hull.size()-1];
        a = hull[hull.size()-2];
      }

      hull.push_back(c);
    }

    for (;;) {
      Point a = hull[hull.size()-2];
      Point b = hull[hull.size()-1];
      Point c = hull[0];
      Point d = hull[1];

      if (b.angle2d(a, c) <= PI) {
        hull.pop_back();
        continue;
      }

      if (c.angle2d(b, d) <= PI) {
        hull.pop_front();
        continue;
      }

      break;
    }

    for (auto p: hull)
      out->push_back(p);
  }

private:
  Point find_midpoint() const {
    Point midpt(0, 0);
    for (auto pt: points)
      midpt += pt;
    midpt /= points.size();
    return midpt;
  }

  const std::vector<Point>& points;
};

using namespace std;

int main() {
  ifstream fin("fc.in");
  ofstream fout("fc.out");

  int N;
  vector<Point> points;
  fin >> N;
  for (int i = 0; i < N; i++) {
    Real x, y;
    fin >> x >> y;
    points.push_back(Point(x, y));
  }

  vector<Point> hull;
  Points(points).find_convex_hull(&hull);

  double total = 0;
  for (int i = 0; i+1 < hull.size(); i++)
    total += hull[i].dist(hull[i+1]);
  total += hull.back().dist(hull[0]);

  fout << std::fixed << std::setprecision(2) << total << endl;
}
