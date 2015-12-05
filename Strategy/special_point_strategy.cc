#include "special_point_strategy.h"
using namespace std;

void SpecialPointStrategy::FindCandidateTrajs(const GridPanel* grid_panel, const Trajectory& traj, double dis, list<int>& candidates) {
	const vector<SamplePoint>& point_list = traj.point_list();
	int point_size = point_list.size();
	SamplePoint& begin_point = point_list[0];
	SamplePoint& end_point = point_list[point_size - 1];	

	GetCandidateTrajs(grid_panel, begin_point, dis, candidates, true);

	list<int> cur_can;
	GetCandidateTrajs(grid_panel, end_point, dis, cur_can, true);
	list<int> res;
	TrajMergeJoin(candidates, cur_can, res);
	candidates.swap(res);

	vector<SamplePoint>& special_point = traj.special_point();
	
	for (vector<SamplePoint>::iterator itor = special_point.begin(); itor != special_point.end() && !candidates.empty(); itor++) {
		list<int> cur_can;
		GetCandidateTrajs(grid_panel, *itor, dis, cur_can, true);
		list<int> res;
		TrajMergeJoin(candidates, cur_can, res);
		candidates.swap(res);
	}
}
