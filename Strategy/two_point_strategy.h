#ifndef TWO_POINT_STRATEGY_H
#define TWO_POINT_STRATEGY_H
#include "strategy.h"

class TwoPointStrategy : public Strategy {
public:
	TwoPointStrategy() { }

	void FindCandidateTrajs(const GridPanel* grid_panel, const Trajectory& traj, double dis, std::list<int>& candidates);

	~TwoPointStrategy() {}
};
#endif
