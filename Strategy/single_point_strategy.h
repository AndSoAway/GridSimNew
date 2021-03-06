#ifndef SINGLE_STRATEGY_H
#define SINGLE_STRATEGY_H
#include "strategy.h"
#include <list>

class SinglePointStrategy : public Strategy {
public:
	SinglePointStrategy() { }
	
	virtual void FindCandidateTrajs(const GridPanel* grid_panel, const Trajectory& traj, double dis, std::list<int>& candidates);

	virtual std::string name() { return "SinglePointStrategy\n"; }	

	~SinglePointStrategy() {}
private:
};
#endif
