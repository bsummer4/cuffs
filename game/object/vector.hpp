#pragma once

namespace vectors {
  template <typename T>
  struct Vector2 {
    T x, y;
    Vector2 <T> () : x(0), y(0) {}
    Vector2 <T> (T x, T y) : x(x), y(y) {}
    T norm() { return hypot(x, y); }
    Vector2 <T> normalized() { return *this / norm(); }
    Vector2 <T> perp() { return Vector2 <T> (y, x); }
    Vector2 <T>  operator-(const Vector2 <T>  other) {
      return Vector2 <T> (x - other.x, y - other.y); }
    bool operator!=(const Vector2 <T>  other) {
      return x != other.x || y != other.y; }
    //Vector2 <T> operator- (const Vector2 <T> other) {
    //  return Vector2 <T> (x - other.x, y - other.y); }
    Vector2 <T> operator+ (const Vector2 <T> other) {
      return Vector2 <T> (x + other.x, y + other.y); }
    Vector2 <T> operator- () { return Vector2 <T> (-x, -y); }
    Vector2 <T> operator/ (T divisor) { return (*this) * (1 / divisor); }
    Vector2 <T> operator* (T times) {
      return Vector2 <T> (x * times, y * times); }};

  struct Vector2_d : public Vector2 <double> {
    double x, y;
    Vector2_d(double x, double y) : Vector2 <double> (x, y) {};
    Vector2_d(Vector2 <int> v) : Vector2 <double>(v.x, v.y) {}};

  struct Point : public Vector2 <int> {
    Point() : Vector2 <int> () {};
    Point(int x, int y) : Vector2 <int> (x, y) {};
    Point(Vector2 <double> v)
      : Vector2 <int> (floor(v.x), floor(v.y)) {}};}
