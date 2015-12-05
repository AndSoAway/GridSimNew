#ifndef STRATEGY_SPECIAL_STRATEGY_H
#define STRATEGY_SPECIAL_STRATEGY_H
#include "strategy.h"

class SpecialPointStrategy: public Strategy {
public:
	SpecialPointStrategy(int begin=0, int end=0): Strategy(begin, end) {
		
	}
 

	virtual void FindCandidateTrajs(const GridPanel* grid_panel, const Trajectory& traj, double dis, std::list<int>& candidates);


	virtual void GetCandidateTrajs(const GridPanel* grid_panel, const SamplePoint& point, double dis, std::list<int>& candidates);

	virtual ~SpecialPointStrategy() { }

private:
	
};
#endif
