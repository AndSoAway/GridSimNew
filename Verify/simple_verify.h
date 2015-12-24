#ifndef SIMPLE_VERIFY_H
#define SIMPLE_VERIFY_H
#include "verify.h"

class SimpleVerify : public BaseVerify {
public:
	double verify(GridPanel& grid_panel, const Trajectory& target, const Trajectory& candidate, double threshold);

	std::string name() { return "SimpleVerify";}
};
#endif
