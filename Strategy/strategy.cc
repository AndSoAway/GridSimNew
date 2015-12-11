#include "strategy.h"
#include <algorithm>
using namespace std;

#define CONVERT_TO_X(x) ((x) / LEN_PER_X)
#define CONVERT_TO_Y(y) ((y) / LEN_PER_Y)

void Strategy::TrajMergeJoin(const list<int>& father_trajs, const std::list<int>& child_trajs, list<int>& join_res) const {
	list<int>::const_iterator fa_itor, ch_itor;
	fa_itor = father_trajs.cbegin();
	ch_itor = child_trajs.cbegin();
	while (fa_itor != father_trajs.cend() && ch_itor != child_trajs.cend()) {
		while (ch_itor != child_trajs.cend()) {
			if (*fa_itor == *ch_itor) {
				join_res.push_back(*fa_itor);
				ch_itor++;
				break;
			} else {
				ch_itor++;
			}
		}
		fa_itor++;
	}
}

void Strategy::TrajMergeUnion(list<int>& father_trajs, const std::list<int>& child_trajs) const {
			list<int>::const_iterator cur_itor, pre_itor;
			cur_itor = child_trajs.cbegin();
			list<int>::iterator can_itor = father_trajs.begin();
			while (cur_itor != child_trajs.cend() && can_itor != father_trajs.end()) {
				while (can_itor != father_trajs.end()) {
					if (*cur_itor == *can_itor) {
						can_itor++;
						break;
					} else if (*cur_itor > *can_itor){
						can_itor++;
					} else {
						father_trajs.insert(can_itor, *cur_itor);
						break;
					}
				}
				pre_itor = cur_itor;
				cur_itor++;
				while (cur_itor != child_trajs.cend() && *cur_itor == *pre_itor) {
					pre_itor = cur_itor;
					cur_itor++; 
				}
			} 
			while (cur_itor != child_trajs.cend()) {
				father_trajs.insert(can_itor, *cur_itor);
				cur_itor++;
			}	
}

void Strategy::GetCandidateTrajs(const GridPanel* grid_panel, const SamplePoint& point, double dis, list<int>& candidates, bool is_end) {
  double x_dif = CONVERT_TO_X(dis);
  double y_dif = CONVERT_TO_Y(dis);
  double x_bottom = point.x() - x_dif ;
  double y_bottom = point.y() - y_dif;
  double x_upper = point.x() + x_dif;
  double y_upper = point.y() + y_dif; 
  pair<int, int> bottom = grid_panel->panel().GetGrid(x_bottom, y_bottom);
  pair<int, int> upper = grid_panel->panel().GetGrid(x_upper, y_upper);
	
	int point_sum = 0;
	for (int i = bottom.first; i <= upper.first; i++) {
		for (int j = bottom.second; j <= upper.second; j++) {
			pair<int, int> grid_index = make_pair(i, j);
			const list<int>& trajs = grid_panel->panel().GetTrajsInGrid(grid_index, is_end);
			string unionInfo = "can size: " + to_string(candidates.size()) +     ", trajs size " + to_string(trajs.size());
 			clock_t cur = clock();

			TrajMergeUnion(candidates, trajs);
			cur = clock() - cur;
			unionInfo += ", union time " + to_string((double)cur / CLOCKS_PE    R_SEC);
 			Log::log(1, unionInfo);
			point_sum += grid_panel->panel().GetPointsInGrid(grid_index).size();
		}
	}
//	printf("Get can_trajs %ld, point size %d\n", candidates.size(), point_sum);
}

void Strategy::FilterEnd(const GridPanel* grid_panel, const Trajectory& traj, std::list<int>& candidates) {
	vector<pair<int, int>> end_grid;
	GetEndGrid(grid_panel, traj, end_grid);
	sort(end_grid.begin(), end_grid.end());

	list<int>::iterator itor = candidates.begin();
	while (itor != candidates.end()) {
		vector<pair<int, int>> can_end_grid;
		GetEndGrid(grid_panel, grid_panel->getTraj(*itor), can_end_grid);
		sort(can_end_grid.begin(), can_end_grid.end());
		if (end_grid[0].first == can_end_grid[0].first && end_grid[0].second == can_end_grid[0].second && end_grid[1].first == can_end_grid[1].first && end_grid[1].second == can_end_grid[1].second) {
			itor++;
		} else {
			itor = candidates.erase(itor);
		}
	}
}

void Strategy::GetEndGrid(const GridPanel* grid_panel, const Trajectory& traj, vector<pair<int, int>>& end_grid) {
	const vector<SamplePoint>& point_list = traj.point_list();
	int point_size = point_list.size();
	const SamplePoint& begin_point = point_list[0];
	const SamplePoint& end_point = point_list[point_size - 1];
	std::pair<int, int> first_grid = grid_panel->panel().GetGrid(begin_point);
	std::pair<int, int> second_grid = grid_panel->panel().GetGrid(end_point);
	end_grid.push_back(first_grid);
	end_grid.push_back(second_grid);
}
