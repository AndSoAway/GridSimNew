#include "strategy.h"
#include <algorithm>
#include "../UnitTest/log.h"
#include "../Tra/utils.h"
#include "../Tra/utils.hpp"
using namespace std;

#define CONVERT_TO_X(x) ((x) / LEN_PER_X)
#define CONVERT_TO_Y(y) ((y) / LEN_PER_Y)

void Strategy::TrajMergeJoin(const list<int>& father_trajs, const std::list<int>& child_trajs, list<int>& join_res) const {
	list<int>::const_iterator fa_itor, ch_itor;
	fa_itor = father_trajs.cbegin();
	ch_itor = child_trajs.cbegin();
	while (fa_itor != father_trajs.cend() && ch_itor != child_trajs.cend()) {
		if (*fa_itor < *ch_itor) ++fa_itor;
		else if (*ch_itor<*fa_itor) ++ch_itor;
		else {
			join_res.insert(join_res.end(), *ch_itor);
			++ch_itor;
			++fa_itor;
		}
	}
}

/*
void Strategy::FindCandidateTrajs(const GridPanel* grid_panel, const Trajectory& traj, double dis, list<int>& candidates) {

}
*/

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

void Strategy::GetCandidateTrajs(const GridPanel* grid_panel, const PointInfo& point, double dis, list<int>& candidates, bool is_end) {
/*	clock_t get_grid = clock();
	int point_sum = 0;
	vector<const list<int>*> neighbour_lists;	
	int total_size = 0;
	string unionInfo;
	int grid_sum = 0;
*/
	const vector<pair<int, int>>& around_grids = point.neighbour_grid_;
	int around_grid_size = around_grids.size();
	for (int i = 0; i < around_grid_size; ++i) {
		const pair<int, int>& grid_index = around_grids[i];
		const list<int>& trajs = grid_panel->panel().GetTrajsInGrid(grid_index, is_end);
		TrajMergeUnion(candidates, trajs);
/*		neighbour_lists.push_back(&trajs);
		total_size += trajs.size();
		grid_sum ++;
		unionInfo += "grid <" + to_string(i) + ", " + to_string(j) + " traj_size: " + to_string(trajs.size()) + "\n";
		point_sum += grid_panel->panel().GetPointsInGrid(grid_index).size();
*/
	}
/*
	get_grid = clock() - get_grid;
	string info = "Get " + to_string(grid_sum) + " Grid : " + to_string((double)get_grid / CLOCKS_PER_SEC * 1000); 
	Log::log(1, info);
	clock_t cur = clock();
	MergeKList(neighbour_lists, candidates);
	cur = clock() - cur;
	unionInfo += "Total traj " + to_string(total_size) + " time: " + to_string((double)cur / CLOCKS_PER_SEC * 1000);
	Log::log(1, unionInfo);
	printf("Get can_trajs %ld, point size %d\n", candidates.size(), point_sum);
*/
}

void Strategy::FindCandidateTrajs(const GridPanel* grid_panel, Trajectory& traj, double dis, std::list<int>& candidates) {
	const Trajectory& con_traj = traj;
	FindCandidateTrajs(grid_panel, con_traj, dis, candidates);
}
/*
int Strategy::GetTotalTrajCount(const GridPanel* grid_panel, const SamplePoint& point, double dis, bool is_end) {
	pair<int, int> grid_index = grid_panel->panel().GetGrid(point);
	return grid_panel->panel().GetAroundTrajCount(grid_index);
}
*/

int Strategy::GetTotalTrajCount(const GridPanel* grid_panel, const SamplePoint& point, double dis, bool is_end) {
	list<int> candidates;	
	const vector<pair<int, int>>& around_grids = point.neighbour_grid_;
	int around_grid_size = around_grids.size();
	for (int i = 0; i < around_grid_size; ++i) {
		const pair<int, int>& grid_index = around_grids[i];
		const list<int>& trajs = grid_panel->panel().GetTrajsInGrid(grid_index, is_end);
		TrajMergeUnion(candidates, trajs);
	}
	return candidates.size();
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
	const vector<PointInfo>& point_list = traj.point_list();
	int point_size = point_list.size();
	const SamplePoint& begin_point = point_list[0];
	const SamplePoint& end_point = point_list[point_size - 1];
	std::pair<int, int> first_grid = grid_panel->panel().GetGrid(begin_point);
	std::pair<int, int> second_grid = grid_panel->panel().GetGrid(end_point);
	end_grid.push_back(first_grid);
	end_grid.push_back(second_grid);
}
