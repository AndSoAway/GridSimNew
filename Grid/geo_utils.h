#ifndef GEO_GEO_UTILS_H_
#define GEO_GEO_UTILS_H_

void SortPoints(const Point& point1, const Point& point2, Point* left_bottom, Point* right_upper);

bool SegIntersectSeg(const Point& seg1_point1, const Point& seg1_point2, const Point& seg2_point1, const Point& seg2_point2);

double CalculateCrossProduct(const Point& p1, const Point& begin, const Point& end);

double CalculateCrossProduct(double a, double b, double c, double d);

double CalculateSegLength(Point begin, Point end);

bool NearZero(double val);

template <typename T> 
void SortNumbers(T* num1, T* num2);

#endif
