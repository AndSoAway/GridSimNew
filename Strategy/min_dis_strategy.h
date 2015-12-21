#ifndef MIN_DIS_STRATEGY_H
#define MIN_DIS_STRATEGY_H

#include "strategy.h"
class MinDisStrategy : public Strategy {
public:
	MinDisStrategy(int k=3) { k_ = k; }
	void FindCandidateTrajs(const GridPanel* grid_panel, const Trajectory& traj, double dis, std::list<int>& candidates);

	virtual void GetCandidateTrajs(const GridPanel* grid_panel, const PointInfo& point, std::list<int>& candidates, bool is_end = false);

	virtual ~MinDisStrategy() { }

protected:
	int k_;

}; 
