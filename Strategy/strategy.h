#ifndef STRATEGY_STRATEGY_H
#define STRATEGY_STRATEGY_H
class Strategy {
public:
	Strategy(int begin=0, int end=0): begin_(begin), end_(end), cur_(0), is_end_(true) {
  index_.push_back(begin); 
  index_.push_back(end);
	end_index_ = index_;
  is_used_ = std::vector<bool>(end + 1, false);
	}

	void candidateFilter(const GridPanel* grid_panel, const Trajectory& traj, std::list<int>& candidates) const = 0;	

	void GetCandidateTrajs(const GridPanel* grid_panel, const SamplePoint& point, double dis, std::list<int>& candidates) const = 0;

	bool HasNext() = 0;
	
	int Next() = 0;

protected:
	int begin_, end_;
	std::vector<int> index_;
	std::vector<int> end_index_; 
	int cur_;
	bool is_end_;
	std::vector<bool> is_used_;
};
#endif
