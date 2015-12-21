#ifndef MIN_DIS_STRATEGY_H
#define MIN_DIS_STRATEGY_H

#include "strategy.h"
class MinDisStrategy : public Strategy {
public:
	MinDisStrategy(int k=3) { k_ = k; }
	void FindCandidateTrajs(const GridPanel* grid_panel, Trajectory& traj, double dis, std::list<int>& candidates);

	void GetCandidateTrajs(const GridPanel* grid_panel, const PointInfo& point_info, std::list<int>& candidates);
	virtual ~MinDisStrategy() { }

protected:
	int k_;

}; 
#endif
