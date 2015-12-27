#ifndef EXPANSION_VERIFY_H
#define EXPANSION_VERIFY_H
#include "verify.h"

class ExpansionVerify: public Verify {
public:
	virtual double verify(GridPanel& grid_panel, const Trajectory& target, const Trajectory& candidate, double threshold);

	virtual std::string name() { return "ExpansionVerify"; }

};

#endif
