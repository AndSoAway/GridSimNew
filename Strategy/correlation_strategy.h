#ifndef CRRL_STRATEGY_H
#define CRRL_STRATEGY_H
#include <list>
#include <string>
#include <utility>
#include "strategy.h"

class CorrelationStrategy : public Strategy {
public:
	void FindCandidateTrajs(const GridPanel* grid_panel, const Trajectory& traj, double dis, std::list<int>& candidates);

	std::string name() { return "correlation strategy\n"; }

private:
	bool GetSameRegion(const std::pair<int, int>&, const std::pair<int, int>&, const RelationInfo*);
};
#endif
