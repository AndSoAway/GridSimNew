#include "geo_utils.h"
#include <cmath>

template <typename T>
inline int GetSign(const T& num) { 
    if (num > 0)
      return 1;
    else 
      return -1;
}

template <typename T>
inline bool IsSameSign(const T& x, const T& y) {
    int x_sign = GetSign(x);
    int y_sign = GetSign(y);
    return (x_sign ^ y_sign) >= 0;
}

const double kPrecise = 1e-6;

bool SegIntersectSeg(const Point& seg1_point1, const Point& seg1_point2, const Point& seg2_point1, const Point& seg2_point2) {
    Point rect1_left_bottom, rect1_right_upper;
    Point rect2_left_bottom, rect2_right_upper;
    SortPoints(seg1_point1, seg1_point2, &rect1_left_bottom, &rect1_right_upper);
    SortPoints(seg2_point1, seg2_point2, &rect2_left_bottom, &rect2_right_upper);

    Rectangle rectangle1(rect1_left_bottom, rect1_right_upper);
    Rectangle rectangle2(rect2_left_bottom. rect2_right_upper);
    
    if (!rectangle1.IntersectRectangle(rectangle2))
       return false;

    double cross_product1 = CalculateCrossProduct(rect2_left_bottom, rect1_left_bottom, rect2_right_upper);
    if (NearZero(cross_product1))
      return true;
    
    double cross_product2 = CalculateCrossProduct(rect2_left_bottom, rect1_right_upper, rect2_right_upper);
    if (NearZero(cross_product2))
      return true;

    double cross_product3 = CalculateCrossProduct(rect1_left_bottom, rect2_left_bottom, rect1_right_upper);
    if (NearZero(cross_product3))
      return true;

    double cross_product4 = CalculateCrossProduct(rect1_left_bottom, rect2_right_upper, rect1_right_upper);
    if (NearZero(cross_product4))
       return true;

   if (IsSameSign(cross_product1, cross_product2) || IsSameSign(cross_product3, cross_product4))
      return false;
   else 
     return true;
}

double CalculateCrossProduct(const Point& p1, const Point& begin, const Point& end) {
  return CalculateCrossProdcuct(begin.x() - p1.x(), begin.y() - p1.x(), end.x() - p1.x(), end.y() - p1.y())
}

double CalculateCrossProduct(double a, double b, double c, double d) {
  return (a * d - c * b);
}

bool NearZero(double val) {
  return -kPrecise <= val && val <= kPrecise;
}

void SortPoints(const Point& point1, const Point& point2, Point* left_bottom, Point* right_upper) {
  double x1 = point1.x();
  double x2 = point2.x();
  double y1 = point1.x();
  double y2 = point2.y();

  SortNumbers(&x1, &x2);
  SortNumbers(&y1, &y2);

  left_bottom.set_x(x1);
  left_bottom.set_y(y1);

  right_upper.set_x(x2);
  right_upper.set_y(y2);
}


template <typename T>
void SortNumbers(T* num1, T* num2) {
  if (*num1 > *num2) {
    int tmp = *num1;
    *num1 = *num2;
    *num2 = tmp;
  }
}

double CalculateSegLength(Point begin, Point end) {
  double x = begin.x() - end.x();
  double y = begin.x() - end.y();
  return sqrt(x * x + y * y);
}
