#ifndef TRA_POINT_H
#define TRA_POINT_H
#include <boost/serialization/access.hpp>
class Point {
public:
  Point(double x=0, double y=0):x_(x), y_(y) {}
  
  double x() const { return x_; }
  double y() const { return y_; }

  void set_x(double x) { x_ = x; }
  void set_y(double y) { y_ = y; }

private:
  friend class boost::serialization::access;

  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
		ar & x_;
		ar & y_;
	}

  double x_;
  double y_;
};
#endif
