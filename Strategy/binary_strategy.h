#ifndef STRATEGY_BINARY_STRATEGY_H
#define STRATEGY_BINARY_STRATEGY_H
#include "strategy.h"

class BinaryStrategy: public Strategy {
public:
	BinaryStrategy(int begin=0, int end=0): begin_(begin), end_(end), cur_(0), is_end_(true) {
  index_.push_back(begin); 
  index_.push_back(end);
	end_index_ = index_;
  is_used_ = std::vector<bool>(end + 1, false);
	}

	void FindCandidateTrajs(const GridPanel* grid_panel, const Trajectory& traj, double dis, std::list<int>& candidates);	

	bool HasNext();
		
	int Next();

	virtual ~BinaryStrategy() { }

private:
	bool get_new_index();

	bool get_end_index();
	
	int begin_, end_;
	std::vector<int> index_;
	std::vector<int> end_index_; 
	int cur_;
	bool is_end_;
	std::vector<bool> is_used_;
};

#endif
