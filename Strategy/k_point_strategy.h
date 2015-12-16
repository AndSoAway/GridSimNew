#ifndef K_POINT_STRATEGY_H
#define K_POINT_STRATEGY_H
#include "strategy.h"
#include <list>

class KPointStrategy : public Strategy {
public:
	KPointStrategy(int k = 1): k_(k){ }
	
	void FindCandidateTrajs(const GridPanel* grid_panel, const Trajectory& traj, double dis, std::list<int>& candidates);
	
	~KPointStrategy() {}
private:
	int k_;
};
#endif
