#ifndef STRATEGY_BINARY_STRATEGY_H
#define STRATEGY_BINARY_STRATEGY_H
#include "Strategy"

class BinaryStrategy: public Strategy {
public:
	BinaryStrategy(int begin=0, int end=0): Strategy(begin, end) {
	}

	void candidateFilter(const GridPanel* grid_panel, const Trajectory& traj, std::list<int>& candidates) const;	

	void GetCandidateTrajs(const GridPanel* grid_panel, const SamplePoint& point, double dis, std::list<int>& candidates) const;

	bool HasNext();
		
	int Next();

	virtual ~BinaryStrategy() { }

private:
	void TrajMergeJoin(std::list<int>& father_trajs, const std::list<int>& child_trajs) const;	

	bool get_new_index();

	bool get_end_index();
	
};

#endif
