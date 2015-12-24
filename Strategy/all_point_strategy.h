#ifndef ALL_POINT_STRATEGY_H
#define ALL_POINT_STRATEGY_H
#include "strategy.h"

class AllPointStrategy : public Strategy {
public:
	AllPointStrategy() { }
	
	virtual void FindCandidateTrajs(const GridPanel* grid_panel, const Trajectory& traj, double dis, std::list<int>& candidates);

	virtual std::string name() { return "AllPointStrategy\n"; }

};

#endif
