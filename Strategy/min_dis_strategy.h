#ifndef MIN_DIS_STRATEGY_H
#define MIN_DIS_STRATEGY_H

#include "strategy.h"
class MinDisStrategy : public Strategy {
public:
	MinDisStrategy(double sim, int k=3): k_(k), sim_threshold_(sim){ }
	void FindCandidateTrajs(const GridPanel* grid_panel, Trajectory& traj, double dis, std::list<int>& candidates);

	void GetCandidateTrajs(const GridPanel* grid_panel, const PointInfo& point_info, std::list<int>& candidates);
	virtual ~MinDisStrategy() { }

protected:
	int k_;
	double sim_threshold_;
}; 
#endif
