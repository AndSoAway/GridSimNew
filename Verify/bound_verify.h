#ifndef BOUND_VERIFY_H
#define BOUND_VERIFY_H
#include "verify.h"

class BoundVerify : public BaseVerify {
public:
	virtual double verify(GridPanel& grid_panel, const Trajectory& target, const Trajectory& candidate, double threshold);

	virtual std::string name() { return "Bound Verify"; }	
};

#endif
