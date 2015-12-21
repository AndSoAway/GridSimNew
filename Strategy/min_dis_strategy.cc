#include <algorithm>
#include "min_dis_strategy.h"
#include "../Tra/utils.hpp"

using namespace std;

void MinDisStrategy::FindCandidateTrajs(const GridPanel* grid_panel, Trajectory& traj, double dis, std::list<int>& candidates) {
	traj.CalPointInfo(grid_panel, dis);
	vector<PointInfo>& point_info = traj.pointinfo_list();
	sort(point_info.begin(), point_info.end(), min_dis_dsc);		
	
	double sum_min_dis = 0;
	int top_k = 0;
	vector<const list<int>*> top_k_lists;
	for (vector<PointInfo>::iterator itor = point_info.begin(); itor != point_info.end() && sum_min_dis < dis; itor++) {
		sum_min_dis += itor->min_dis_;
		top_k++;
		pair<int, int>& grid_index = itor->grid_index_;
		const list<int>& tra_list = grid_panel->panel().GetTrajsInGrid(grid_index, false);
		top_k_lists.push_back(&tra_list);
	}
	MergeKList(top_k_lists, candidates);	

	vector<pair<int, int>> point_grid;
	int point_size = point_info.size();
	for (int i = 0; i < point_size; i++) {
		int traj_size = grid_panel->panel().GetTrajCountInGrid(point_info[i].grid_index_); 
		point_grid.push_back(make_pair(i, traj_size));
	}	
	sort(point_grid.begin(), point_grid.end(), order_by_count);

	for (int index = 0; index < k_ && index < point_size; index++) {
		int chose_index = point_grid[index].first;
		const PointInfo &chose_info = point_info[chose_index];
		list<int> sec_can;
		GetCandidateTrajs(grid_panel, chose_info, sec_can);
		
		list<int> res;
		TrajMergeJoin(candidates, sec_can, res);
		candidates.swap(res);
	}	
}

void MinDisStrategy::GetCandidateTrajs(const GridPanel* grid_panel, const PointInfo& point_info, list<int>& candidates) {
	const vector<pair<int, int>>& grid_index = point_info.neighbour_grid_;
	for(vector<pair<int, int>>::const_iterator itor = grid_index.cbegin(); itor != grid_index.cend(); itor++) {
		const list<int>& trajs = grid_panel->panel().GetTrajsInGrid(*itor, false);
		TrajMergeUnion(candidates, trajs);
	}
}
