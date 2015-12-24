#ifndef EARLY_VERIFY_H
#define EARLY_VERIFY_H
#include "verify.h"

class EarlyVerify : public Verify {
public:
	double verify(const GridPanel *grid_panel, const Trajectory& target, const Trajectory &candidate, double threshold);
};
#endif
