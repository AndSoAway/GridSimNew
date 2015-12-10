#ifndef STRATEGY_SPECIAL_STRATEGY_H
#define STRATEGY_SPECIAL_STRATEGY_H
#include "strategy.h"
#include <vector>
#include <utility>

class SpecialPointStrategy: public Strategy {
public:
	SpecialPointStrategy(int begin=0, int end=0): Strategy() {
	}
 
	virtual void FindCandidateTrajs(const GridPanel* grid_panel, const Trajectory& traj, double dis, std::list<int>& candidates);

	virtual ~SpecialPointStrategy() { }
};
#endif
