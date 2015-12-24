#ifndef __VERIFY_VERIFY_H
#define __VERIFY_VERIFY_H
#include <string>
#include "../Tra/trajectory.h"
#include "../Tra/sample_point.h"
class BaseVerify {
public:
	virtual	double verify(GridPanel& grid_panel, const Trajectory& target, const Trajectory& candidate, double threshold) = 0;

	virtual std::string name() = 0;
};
#endif
