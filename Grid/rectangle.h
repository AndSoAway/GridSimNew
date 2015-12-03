#ifndef GRID_RECTANGLE_H_
#define GRID_RECTANGLE_H_ 1
#include "../Tra/point.h"

const int kChildCount = 4;

class Rectangle {
public:
  Rectangle():splitted_(false) { }

  Rectangle(const Point& left_bottom, const Point& right_upper) : left_bottom_(left_bottom), right_upper_(right_upper), splitted_(false) { 
    double center_x = (left_bottom.x() + right_upper.x()) / 2;
    double center_y = (left_bottom.y() + right_upper.y()) / 2;
    center_point_ = Point(center_x, center_y);
  }
  
  bool ContainPoint(const Point& point) const;

  bool IntersectSegment(const Point& point1, const Point& point2)const;

  bool IntersectRectangle(const Rectangle& rectangle)const;

  
  const Point left_bottom() const { return left_bottom_; }
  const Point right_upper() const { return right_upper_; }
  //Rectangle* SpiltNode();

  Point center_point() { return center_point_; } 

private:
  Point left_bottom_;
  Point right_upper_;
  Point center_point_;
  bool splitted_;
//  Rectangle children_[kChildCount];
};
#endif
