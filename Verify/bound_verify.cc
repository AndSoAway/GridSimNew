#include "bound_verify.h"
#include <algorithm>

using namespace std;

bool sort_by_min(const pair<double, int>& left, const pair<double, int>& right) {
	return left.first < right_first;
}

bool sort_by_max(const pair<double, int>& left, const pair<double, int>& right) {
	return left.first > right.first;
}

double BoundVerify::verify(GridPanel& grid_panel, const Trajectory& target, const Trajectory& candidate, double threshold) {
	double total_min_dist = 0;
	
	const vector<PointInfo>& tr_point_list = target.point_list();
	int n = tr_point_list.size();

	const vector<PointInfo>& can_point_list = candidate.point_list();
	int m = can_point_list.size();

	double thres_dis = (1 - threshold) * (n + m) * DMAX;

	unordered_map<int, vector<pair<double, int>>>& target_min_sort_grid;
	unordered_map<int, vector<pair<double, int>>>& target_max_sort_grid;
	unordered_map<int, vector<pair<double, int>>>& can_min_sort_grid;
	unordered_map<int, vector<pair<double, int>>>& can_max_sort_grid;

	getDis(target_min_sort_grid, target_max_sort_grid, tr_point_list);	
	getDis(can_min_sort_grid, can_max_sort_grid, can_point_list);

	int target_max_grid_size = GetMaxGridSize(tr_point_list);
	int can_max_grid_size = GetMaxGridSize(can_point_list);
	
	
}

int BoundVerify::GetMaxGridSize() {
	int max_grid_size = -1;
	for (int i = 0; i < n; ++i) {
		PointInfo& point = tr_point_list[i];
		int around_grid_size = point.neighbour_grid_.size();
		max_grid_size = max(around_grid_size, max_grid_size);
	}
	return max_grid_size;
}

double getLowerAndUpper(const Trajectory& target, unordered_map<int, vector<pair<double, int>>>& target_min_sort_grid, unordered_map<int, vector<pair<double, int>>>& target_max_sort_grid, double *total_lower_dis, double *total_upper_dis, int grid_index) {
	const vector<PointInfo>& tr_point_list = target.point_list();
	int n = tr_point_list.size();

	total_lower_dis = 0;
	total_upper_dis = 0;
	for (int point_index = 0; point_index < n; ++point_index) {
		int tra_id = target.id();
		PointInfo& point_info = tr_point_list[point_index];

		//Get the ith grid in the around grids of point 
		const vector<pair<double, int>>& dis_grid = target_min_sort_grid[point_index];
		int cur_grid_size = dis_grid.size();
		if (cur_grid_size - 1 < i) {
			*total_lower_dis = ;
			*total_upper_dis = ;
			return;
		}
		double cur_min_dis = dis_grid[grid_index].first;
		int ori_grid_index = dis_grid[grid_index].second;
			//Get the current grid_index and get the distance from point to the lines
		const pair<int, int>& cur_grid_index = point_info.neighbour_grid_[ori_grid_index];
		double min_dis = DBL_MAX;
		Panel::segmentset& intersect_sgements = grid_panel.panel().GetSegmentsInGrid(cur_grid_index, tra_id);
		if (!intersect_segments.empty()) {
			Panel::segmentset::iterator end_itor = intersect_segents.end();
			for (Panel::segmenrtset::iterator itor = intersect_segments.begin(); itor != end_itor; ++itor) {
				const pair<int, int>& cur_seg = *itor;
				double cur_min_dis = GetMinDistance(point_info, candidate, cur_seg);
				if (cur_min_dis < min_dis) {
					min_dis = cur_min_dis;
				}
			}	
		}
			//Get the lower and upper bound
		double lower_bound = min(point_min_dis[point_index], min_dis);
		double upper_bound = min(point_max_dis[point_index], max_dis);
		
		total_lower_dis += lower_bound;
		total_upper_dis += upper_bound;
		
		if (total_lower_dis > thres_dis) 
			return OVERMINDIS;					
	}	
	
}

void BoundVerify::getDis(unordered_map<int, vector<pair<int, int>>>& sort_min, unordered_map<int, vector<pair<int, int>>>& sort_max, const vector<PointInfo>& point_list) {
	int n = point_list.size();
	for (int i = 0; i < n; ++i) {
		const PointInfo& point = point_list[i];
		vector<pair<double, int>> min_dis;
		vector<pair<double, int>> max_dis;
		getMinDis(point, min_dis);
		getMaxDis(point, max_dis);		
		sort(min_dis.begin(), min_dis.end(), sort_by_min);
		sort(max_dis.begin(), max_dis.end(), sort_by_max);
		sort_min[i] = min_dis;
		sort_max[i] = max_dis; 
	} 
}

