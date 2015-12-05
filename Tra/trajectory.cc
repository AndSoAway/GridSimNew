#include "trajectory.h"
#include "config.h"
#include "utils.h"
#include <cmath>

#define POINTMINSIZE 4

void Trajectory::PushPoint(SamplePoint& point) {
  point.set_tra(this);
  point_list_.push_back(point);
	UpdateSpecialPoint();
}

void Trajectory::UpdateSpecialPoint() {
	int size = point_list_.size();
	if (size < POINTMINSIZE)
		return;
	
	const Point& first = point_list_[size - 4].point();
	const Point& second = point_list_[size - 3].point();
	const Point& third = point_list_[size - 2].point();
	
	double a = distSimplify(first.x(), first.y(), second.x(), second.y());

	double b = distSimplify(second.x(), second.y(), third.x(), third.y());
	
	double c = distSimplify(first.x(), first.y(), third.x(), third.y());

	double cos_val = (a * a + b * b - c* c) / (2 * a * b);

	double angle = acos(cos_val) * 180 / PI;

  if (angle <= ANGLETHRESHOLD) {
		special_point_.push_back(point_list_[size - 2]);	
	}
}

int Trajectory::tra_counter = 1;
