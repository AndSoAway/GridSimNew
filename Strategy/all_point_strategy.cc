#include "all_point_strategy.h"

using namespace std;

void AllPointStrategy::FindCandidateTrajs(const GridPanel* grid_panel, const Trajectory& traj, double dis, list<int>& candidates) {
	const vector<PointInfo>& point_list = traj.point_list();
	int point_size = point_list.size();

	const PointInfo& chose_point = point_list[0];
	const pair<int, int>& grid_index = chose_point.grid_index_;
	candidates = grid_panel->panel().GetTrajsAroundGrid(grid_index);
	
	for(int i = 1; i < point_size; i++) {
		list<int>  res;
		const PointInfo& chose_point = point_list[i];
		const pair<int, int>& grid_index = chose_point.grid_index_;
		const list<int>& cur_can = grid_panel->panel().GetTrajsAroundGrid(grid_index);
		TrajMergeJoin(candidates, cur_can, res);
		candidates.swap(res);
	}	
}
