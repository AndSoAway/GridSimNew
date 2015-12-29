#include <algorithm>
#include <utility>
#include "min_dis_strategy.h"
#include "../Tra/utils.hpp"

using namespace std;

bool order_second_des(const pair<int, double>& lft, const pair<int, double>& rhs) {
	return lft.second > rhs.second;
}

void MinDisStrategy::FindCandidateTrajs(const GridPanel* grid_panel, const Trajectory& traj, double dis, std::list<int>& candidates) {
//	traj.CalPointInfo(grid_panel, dis);
	const vector<PointInfo>& point_info = traj.pointinfo_list();
	int point_info_size = point_info.size();
	double sim_dis = 2 * (1 - sim_threshold_) * point_info_size * DMAX; 

	vector<pair<int, double>> index_min;
	for (int i = 0; i < point_info_size; i++) {
		index_min.push_back(make_pair(i, point_info[i].min_dis_));
	}
	
	sort(index_min.begin(), index_min.end(), order_second_des);		
	
	double sum_min_dis = 0;
	int top_k = 0;
	for (int i = 0; i < point_info_size && sum_min_dis < sim_dis; ++i) {
	//	printf("min_dis_ %lf, sum_min_dis %lf\n", itor->min_dis_, sum_min_dis);
		sum_min_dis += index_min[i].second;
		top_k++;
	}

	if (sum_min_dis < sim_dis) {
//		vector<const list<int>*> top_k_lists;
		for (int i = 0; i < top_k; i++) {
			int index = index_min[i].first;
			const pair<int, int>& grid_index = point_info[index].grid_index_;
			const list<int>& tra_list = grid_panel->panel().GetTrajsInGrid(grid_index, false);
//			top_k_lists.push_back(&tra_list);
			TrajMergeUnion(candidates, tra_list);
		}
//		MergeKList(top_k_lists, candidates);	
	}	

//	printf("top_k is %d\n", top_k);
//	printf("Top-%d Candidate: %d\n", top_k, candidates.size());
	vector<pair<int, int>> point_grid;
	int point_size = point_info.size();
	for (int i = 0; i < point_size; ++i) {
		int traj_size = point_info[i].traj_count_in_grid_; 
		point_grid.push_back(make_pair(i, traj_size));
	}	
	sort(point_grid.begin(), point_grid.end(), order_by_count);

	for (int index = 0; index < k_ && index < point_size; ++index) {
		int chose_index = point_grid[index].first;
		const PointInfo &chose_info = point_info[chose_index];
		list<int> cur_can;
		GetCandidateTrajs(grid_panel, chose_info, dis, cur_can, false);
//		printf("%dth point, ground trajs %d\n", index + 1, sec_can.size());		
		list<int> res;
		TrajMergeJoin(candidates, cur_can, res);
		candidates.swap(res);
//		printf("Join %dth point, candidates %d\n", index + 1, candidates.size());
	}	

}

