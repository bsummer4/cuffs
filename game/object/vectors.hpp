#pragma once
#include <cmath>

namespace vectors {
  using namespace std;
  template <typename T>
  struct Vector2 {
    T x, y;
    Vector2 <T> () : x(0), y(0) {}
    Vector2 <T> (T x, T y) : x(x), y(y) {}
    Vector2 <T> (const Vector2 <T> &v) : x(v.x), y(v.y) {}
    T norm() { return hypot(x, y); }
    Vector2 <T> normalized() { return *this / norm(); }
    Vector2 <T> perp() { return Vector2 <T> (-y, x); }
    bool operator==(const Vector2 <T> &other) {
      return x == other.x && y == other.y; }
    bool operator!=(const Vector2 <T> &other) { return !((*this) == other); }
    Vector2 <T> operator- () { return Vector2 <T> (-x, -y); }
    Vector2 <T>  operator-(const Vector2 <T> &other) {
      return Vector2 <T> (x - other.x, y - other.y); }
    Vector2 <T> operator+ (const Vector2 <T> &other) {
      return Vector2 <T> (x + other.x, y + other.y); }
    Vector2 <T> operator/ (T divisor) { return (*this) * (1 / divisor); }
    Vector2 <T> operator* (T times) {
      return Vector2 <T> (x * times, y * times); }};

  struct Vector2_d : public Vector2 <double> {
    Vector2_d(double x, double y) : Vector2 <double> (x, y) {}
    Vector2_d(const Vector2 <int> &v) : Vector2 <double> (v.x, v.y) {}
    Vector2_d(const Vector2 <double> &v) : Vector2 <double> (v.x, v.y) {}
    Vector2 <int> floor() {
      int x_ = std::floor(x), y_ = std::floor(y);
      return Vector2 <int> (x_, y_); }};

  typedef Vector2 <int> Point; }
