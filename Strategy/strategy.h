#ifndef STRATEGY_STRATEGY_H
#define STRATEGY_STRATEGY_H
#include <vector>
#include <list>

#include "../Grid/grid_panel.h"

#include "../Tra/trajectory.h"
#include "../Tra/sample_point.h"

class GridPanel;

class Strategy {
public:
	Strategy(int begin=0, int end=0): begin_(begin), end_(end), cur_(0), is_end_(true) {
  index_.push_back(begin); 
  index_.push_back(end);
	end_index_ = index_;
  is_used_ = std::vector<bool>(end + 1, false);
	}

	virtual void FindCandidateTrajs(const GridPanel* grid_panel, const Trajectory& traj, double dis, std::list<int>& candidates) = 0;	

	virtual void GetCandidateTrajs(const GridPanel* grid_panel, const SamplePoint& point, double dis, std::list<int>& candidates) = 0;

	virtual bool HasNext() = 0;
	
	virtual int Next() = 0;

	virtual ~Strategy() { }
protected:
	int begin_, end_;
	std::vector<int> index_;
	std::vector<int> end_index_; 
	int cur_;
	bool is_end_;
	std::vector<bool> is_used_;
};
#endif
