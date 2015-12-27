#ifndef SIGNATURE_VERIFY_H
#define SIGNATURE_VERIFY_H
#include "verify.h"
#include "../Grid/grid_panel.h"

class SignatureVerify : public BaseVerify {
public:
	double verify(GridPanel& grid_panel, const Trajectory& target, const Trajectory& candidate, double threshold);
	
	std::string name() { return "SignatureVerify\n"; }
private:
	bool GetMinDistance(GridPanel&, const PointInfo&, const Trajectory&, double *);
	
	double GetMinDistance(const PointInfo&, const Trajectory&, const std::pair<int, int>&);
}; 
#endif
