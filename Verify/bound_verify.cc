#include "bound_verify.h"
#include "../Tra/config.h"
#include "../Grid/panel.h"
#include "../Grid/grid_panel.h"
#include <algorithm>
#include <cfloat> 

using namespace std;

bool sort_by_min(const pair<double, int>& left, const pair<double, int>& right) {
	return left.first < right.first;
}

bool sort_by_max(const pair<double, int>& left, const pair<double, int>& right) {
	return left.first > right.first;
}

double BoundVerify::verify(GridPanel& grid_panel, const Trajectory& target, const Trajectory& candidate, double threshold) {
	const vector<PointInfo>& tr_point_list = target.point_list();
	int n = tr_point_list.size();

	const vector<PointInfo>& can_point_list = candidate.point_list();
	int m = can_point_list.size();

	double thres_dis = (1 - threshold) * (n + m) * DMAX;
	
	int tar_id = target.id();
	int can_id = candidate.id();
	
	unordered_map<int, vector<pair<double, int>>> target_min_sort_grid;
	vector<double> target_max_first_grid;
	unordered_map<int, vector<pair<double, int>>> can_min_sort_grid;
	vector<double> can_max_first_grid;

	bool hasIntersect = getDis(grid_panel, can_id, target_min_sort_grid, target_max_first_grid, tr_point_list);	
	if (hasIntersect == false)
		return OVERMINDIS;

	hasIntersect = getDis(grid_panel, tar_id, can_min_sort_grid, can_max_first_grid, can_point_list);

	if (hasIntersect == false)
		return OVERMINDIS;

	int target_max_grid_size = GetMaxGridSize(tr_point_list);
	int can_max_grid_size = GetMaxGridSize(can_point_list);
	

	double target_lower_dis, target_upper_dis;
	double can_lower_dis, can_upper_dis;
	double total_lower_dis, total_upper_dis;
	vector<pair<bool, double>> target_true_dis(n, make_pair(false, DBL_MAX));
	vector<pair<bool, double>> can_true_dis(m, make_pair(false, DBL_MAX));
	for (int i = 0; i < target_max_grid_size || i < can_max_grid_size; i++) {
		getLowerAndUpper(grid_panel, target, candidate, can_id, target_true_dis, target_min_sort_grid, target_max_first_grid, &target_lower_dis, &target_upper_dis, i);
		getLowerAndUpper(grid_panel, candidate, target, tar_id, can_true_dis, can_min_sort_grid, can_max_first_grid, &can_lower_dis, &can_upper_dis, i);
		total_lower_dis = target_lower_dis + can_lower_dis;
		total_upper_dis = target_upper_dis + can_upper_dis;
		if (total_lower_dis > thres_dis)
			return OVERMINDIS;
		if (total_upper_dis < thres_dis) 
			return 1.0; 
		if (abs(total_lower_dis - total_upper_dis) < PRECISE) {
			break;
		}
	}	
	double res = 1 -  (total_lower_dis + total_upper_dis) / (2 * (n + m) * DMAX);
	return res;			
}

int BoundVerify::GetMaxGridSize(const vector<PointInfo>& tr_point_list) {
	int max_grid_size = -1;
	int n = tr_point_list.size();
	for (int i = 0; i < n; ++i) {
		const PointInfo& point = tr_point_list[i];
		int around_grid_size = point.neighbour_grid_.size();
		max_grid_size = max(around_grid_size, max_grid_size);
	}
	return max_grid_size;
}

double BoundVerify::getLowerAndUpper(GridPanel& grid_panel, const Trajectory& target, const Trajectory& candidate, int tra_id, vector<pair<bool, double>>& true_dis, unordered_map<int, vector<pair<double, int>>>& target_min_sort_grid, vector<double>& target_first_max, double *total_lower_dis, double *total_upper_dis, int grid_index) {
	const vector<PointInfo>& tr_point_list = target.point_list();
	int n = tr_point_list.size();

	total_lower_dis = 0;
	total_upper_dis = 0;
	for (int point_index = 0; point_index < n; ++point_index) {
		const PointInfo& point_info = tr_point_list[point_index];

		//Given the point_index'th point, Get the min_dis and max_dis of grid_index'th grid

		//if the grid_index exceeds the dis_grid, conduct the true_dis[point_index]  
		const vector<pair<double, int>>& min_dis_grid = target_min_sort_grid[point_index];
		int dis_grid_size = min_dis_grid.size();
		if (true_dis[point_index].first == true) {
			*total_lower_dis += true_dis[point_index].second;
			*total_upper_dis = true_dis[point_index].second;
			continue;
		}

		if (grid_index == 0) {
			*total_lower_dis += min_dis_grid[grid_index].first;
			*total_upper_dis += target_first_max[point_index]; 
			continue;
		}

		int pre_ori_grid_index = min_dis_grid[grid_index - 1].second;
		const pair<int, int>& cur_grid_index = point_info.neighbour_grid_[pre_ori_grid_index];
		double dis_point_seg = DBL_MAX;
		Panel::segmentset& intersect_segments = grid_panel.panel().GetSegmentsInGrid(cur_grid_index, tra_id);
		Panel::segmentset::iterator end_itor = intersect_segments.end();
		for (Panel::segmentset::iterator itor = intersect_segments.begin(); itor != end_itor; ++itor) {
			const pair<int, int>& cur_seg = *itor;
			double cur_min_dis = GetMinDistance(point_info, candidate, cur_seg);
			if (cur_min_dis < dis_point_seg) {
				dis_point_seg = cur_min_dis;
			}
		}
			
		dis_point_seg = min(true_dis[point_index].second, dis_point_seg);
		double cur_min_dis = DBL_MAX;
		if (grid_index < dis_grid_size) {
			cur_min_dis = min_dis_grid[grid_index].first;
		}
		if (cur_min_dis >= dis_point_seg) {
			*total_lower_dis += dis_point_seg;
			*total_upper_dis += dis_point_seg;
			true_dis[point_index].first = true;
		} else {
			*total_lower_dis += cur_min_dis;
			*total_upper_dis += dis_point_seg;
		}
		true_dis[point_index].second = dis_point_seg;
	}	
	return (*total_lower_dis + *total_upper_dis) / 2;
}

bool BoundVerify::getDis(GridPanel& grid_panel, int test_id, unordered_map<int, vector<pair<double, int>>>& sort_min, vector<double>& first_max, const vector<PointInfo>& point_list) {
	int n = point_list.size();
	for (int i = 0; i < n; ++i) {
		const PointInfo& point = point_list[i];
		vector<pair<double, int>> min_dis;
		getMinDis(grid_panel, test_id, point, min_dis);
		
		if (min_dis.empty())
			return false;
		sort(min_dis.begin(), min_dis.end(), sort_by_min);
		sort_min[i] = min_dis;
		
		int first_grid_index = min_dis[0].second;
		double max_dis = getMaxDis(first_grid_index, point);	
		first_max[i] = max_dis;
	} 
	return true;
}

double BoundVerify::GetMinDistance(const PointInfo& target, const Trajectory& candidates, const pair<int, int>& cur_seg) {
	const vector<PointInfo>& point_list = candidates.point_list();
	
	int begin = cur_seg.first;
	int end = cur_seg.second;
	//int size = point_list.size();
//	printf("segment begin %d, end %d, tra size %d\n", begin, end, size);
	double min_dis = DBL_MAX;
	for (int i = begin; i < end; i++) {
		double cur_dis = minDistance(target, point_list[i], point_list[i + 1]);
		if (cur_dis < min_dis) {
			min_dis = cur_dis;
		}	
	} 		
//	printf("new min_dis is %lf\n", min_dis);
	return min_dis;
}
