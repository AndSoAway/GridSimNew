#include "rectangle.h"
#include "geo_utils.h"

bool Rectangle::ContainPoint(const Point& point) const {
  return left_bottom_.x() <= point.x() && point.x() <= right_upper_.x() && left_bottom_.y() <= point.y() && point.y() <= right_upper_.y();
}

bool Rectangle::IntersectSegment(const Point& begin_point, const Point& end_point) const {
  if (ContainPoint(begin_point) || ContainPoint(end_point)) 
    return true;

  Point left_upper(left_bottom_.x(), right_upper_.y());
  Point right_bottom(right_upper_.x(), left_bottom_.y());

  bool is_intersect = SegIntersectSeg(left_upper, right_bottom, begin_point, end_point) || SegIntersectSeg(left_bottom, right_upper, begin_point, end_point);
 
  return is_intersect;
}

Rectangle* SplitNode() {
  if (splitted_)
    return children_;
  
  Point center_point;
  FindCenterPoint(&center_point);
  Point left_center(left_bottom.x(), center_point.y());
  Point center_upper(center_point.x(), right_upper.y());
  Point center_bottom(center_point.x(), left_bottom.y());
  Point right_center(right_upper.x(), center_point.y());

  Rectangle left_upper_rect(left_center, center_upper);
  Rectangle left_bottom_rect(left_bottom_, center_point);
  Rectangle right_upper_rect(center_point, right_upper_);
  Rectangle right_bottom_rect(center_bottom, right_center);

  children[0] = left_upper_rect;
  children[1] = left_bottom_rect;
  children[2] = right_upper_rect;
  children[3] = right_bottom_rect;

  return (Rectangle*)children;
}
