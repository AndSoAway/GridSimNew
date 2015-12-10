#ifndef END_POINT_STRATEGY_H
#define END_POINT_STRATEGY_H
#include "strategy.h"
#include "../Grid/grid_panel.h"

class EndPointStrategy: public Strategy {
public:
	EndPointStrategy() {}

	void FindCandidateTrajs(const GridPanel* grid_panel, const Trajectory& traj, double dis, std::list<int>& candidates); 

 	~EndPointStrategy() {} 
};

#endif
