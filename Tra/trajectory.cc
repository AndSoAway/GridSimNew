#include "trajectory.h"

void Trajectory::PushPoint(SamplePoint& point) {
  point.set_tra(this);
//printf("point's id %d\n", point.tra()->id());
  point_list_.push_back(point);
}

/*void Trajectory::PushPoint(double x, double y) {
  point_list_.push_back(Point(x, y));
}
*/

int Trajectory::tra_counter = 1;
