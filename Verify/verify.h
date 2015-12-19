#ifndef __VERIFY_VERIFY_H
#define __VERIFY_VERIFY_H
#include "../Tra/trajectory.h"
#include "../Tra/sample_point.h"

double verify(const Trajectory& target, const Trajectory& candidate, double threshold);

double minDistance(const SamplePoint& point, const Trajectory& tra);

double minDistance(const SamplePoint& point, const SamplePoint& begin, const SamplePoint& end);
#endif
