#ifndef ENDPOINT_ASCOMMON_STRATEGY_H
#define ENDPOINT_ASCOMMON_STRATEGY_H
#include "strategy.h"
class EndAsCommonStrategy : public Strategy {
public:
	EndAsCommonStrategy() { }
	
	void FindCandidateTrajs(const GridPanel* grid_panel, const Trajectory& traj, double dis, std::list<int>& candidates);

	~EndAsCommonStrategy() { }
};
#endif
