#ifndef SIMPLE_VERIFY_H
#define SIMPLE_VERIFY_H
#include "verify.h"

class SimpleVerify : public Verify {
public:
	double verify(const GridPanel* grid_panel, const Trajectory& target, const Trajectory& candidate, double threshold);
};
#endif
