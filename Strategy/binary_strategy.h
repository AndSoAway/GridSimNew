#ifndef STRATEGY_BINARY_STRATEGY_H
#define STRATEGY_BINARY_STRATEGY_H
#include "strategy.h"

class BinaryStrategy: public Strategy {
public:
	BinaryStrategy(int begin=0, int end=0): Strategy(begin, end) {
	}

	void FindCandidateTrajs(const GridPanel* grid_panel, const Trajectory& traj, double dis, std::list<int>& candidates);	

	void GetCandidateTrajs(const GridPanel* grid_panel, const SamplePoint& point, double dis, std::list<int>& candidates);

	bool HasNext();
		
	int Next();

	virtual ~BinaryStrategy() { }

private:
	void TrajMergeJoin(const std::list<int>& father_trajs, const std::list<int>& child_trajs, std::list<int>& res) const;	

	bool get_new_index();

	bool get_end_index();
	
};

#endif
