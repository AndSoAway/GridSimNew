#ifndef STRATEGY_SPECIAL_STRATEGY_H
#define STRATEGY_SPECIAL_STRATEGY_H
#include "strategy.h"
#include <vector>
#include <utility>

class SpecialPointStrategy: public Strategy {
public:
	SpecialPointStrategy(): Strategy() {
	}
 
	virtual void FindCandidateTrajs(const GridPanel* grid_panel, const Trajectory& traj, double dis, std::list<int>& candidates);

	virtual ~SpecialPointStrategy() { }
private:
	static int special_point_size_;
	static int joined_tra_count_;
	static int total_point_size_;
};
#endif
