#ifndef __VERIFY_VERIFY_H
#define __VERIFY_VERIFY_H
#include "../Tra/trajectory.h"
#include "../Tra/sample_point.h"
class Verify {
public:
	virtual	double verify(const GridPanel *grid_panel, const Trajectory& target, const Trajectory& candidate, double threshold) = 0;
};
#endif
