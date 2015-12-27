#ifndef EARLY_VERIFY_H
#define EARLY_VERIFY_H
#include "verify.h"

class EarlyVerify : public BaseVerify {
public:
	virtual double verify(GridPanel& grid_panel, const Trajectory& target, const Trajectory &candidate, double threshold);

	virtual std::string name() {  return "EarlyVerify"; }
};
#endif
