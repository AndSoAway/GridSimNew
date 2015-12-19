#include "special_point_strategy.h"
#include "../UnitTest/log.h"
#include <algorithm>
using namespace std;
int SpecialPointStrategy::special_point_size_ = 0;
int SpecialPointStrategy::joined_tra_count_ = 0;
int SpecialPointStrategy::total_point_size_ = 0;
void SpecialPointStrategy::FindCandidateTrajs(const GridPanel* grid_panel, const Trajectory& traj, double dis, list<int>& candidates) {
	const vector<PointInfo>& point_list = traj.point_list();
	int point_size = point_list.size();
	const SamplePoint& begin_point = point_list[0];
	const SamplePoint& end_point = point_list[point_size - 1];	
	GetCandidateTrajs(grid_panel, begin_point, dis, candidates, false);
	
	list<int> cur_can;
	GetCandidateTrajs(grid_panel, end_point, dis, cur_can, false);

	list<int> res;
	TrajMergeJoin(candidates, cur_can, res);
	candidates.swap(res);

	const vector<SamplePoint>& special_point = traj.special_point();
	special_point_size_ += special_point.size();
	joined_tra_count_++;
	total_point_size_ += point_size;
	
	if (joined_tra_count_ % 10000 == 0) {
		string spec_info = "Joined Tra Count: " + to_string(joined_tra_count_) + ", total point size:" + to_string(total_point_size_) +" , special_point_size: " + to_string(special_point_size_);
		Log::log(1,spec_info);
	}	
	for (vector<SamplePoint>::const_iterator itor = special_point.cbegin(); itor != special_point.cend() && !candidates.empty(); itor++) {
		list<int> cur_can;
		GetCandidateTrajs(grid_panel, *itor, dis, cur_can, false);
		list<int> res;
		TrajMergeJoin(candidates, cur_can, res);
		candidates.swap(res);
	}
}
