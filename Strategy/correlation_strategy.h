#ifndef CRRL_STRATEGY_H
#define CRRL_STRATEGY_H
#include <list>
#include <string>
#include "strategy.h"

class CorrelationStrategy : public Strategy {
public:
	void FindCandidateTrajs(const GridPanel* grid_panel, Trajectory& traj, double dis, std::list<int>& candidates);

	std::string name() { return "correlation strategy\n"; }

};
#endif
