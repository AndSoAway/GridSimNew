#ifndef EARLY_VERIFY_H
#define EARLY_VERIFY_H
#include "verify.h"

class EarlyVerify : public BaseVerify {
public:
	double verify(GridPanel& grid_panel, const Trajectory& target, const Trajectory &candidate, double threshold);

	std::string name() {  return "EarlyVerify"; }
};
#endif
