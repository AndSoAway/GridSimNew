#ifndef EXPANSION_VERIFY_H
#define EXPANSION_VERIFY_H
#include "verify.h"

class ExpansionVerify: public BaseVerify {
public:
	virtual double verify(GridPanel& grid_panel, const Trajectory& target, const Trajectory& candidate, double threshold);

	virtual std::string name() { return "ExpansionVerify"; }

private:
	bool GetMinDistance(GridPanel&, const PointInfo&, const Trajectory&, double *);
	
	double GetMinDistance(const PointInfo&, const Trajectory&, const std::pair<int, int>&);
};

#endif
