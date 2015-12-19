#ifndef TRA_POINT_H
#define TRA_POINT_H
class Point {
public:
  Point(double x=0, double y=0):x_(x), y_(y) {}
  
  double x() const { return x_; }
  double y() const { return y_; }

  void set_x(double x) { x_ = x; }
  void set_y(double y) { y_ = y; }

private:
  double x_;
  double y_;
};
#endif
