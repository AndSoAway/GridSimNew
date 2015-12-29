#include "expansion_verify.h"
#include <cfloat>
#include <cmath>
#include <algorithm>
#include "../Tra/utils.h"
#include "../Grid/panel.h"
#include "../Grid/grid_panel.h"

using namespace std;

bool sort_by_dis(const pair<double, int>& left, const pair<double, int>& right) {
	return left.first < right.first;
}

double ExpansionVerify::verify(GridPanel& grid_panel, const Trajectory& target, const Trajectory& candidate, double threshold) {
	double total_min_dist = 0;

	const vector<PointInfo>& tr_point_list = target.point_list();
	int n = tr_point_list.size();

	const vector<PointInfo>& can_point_list = candidate.point_list();
	int m = can_point_list.size();

	double thres_dis = (1 - threshold) * (n + m) * DMAX;

	for (int index = 0; index < n && total_min_dist <= thres_dis; index++) {
		const PointInfo& target_point = tr_point_list[index];
		double min_dis;
		bool valid = GetMinDistance(grid_panel, target_point, candidate, &min_dis);
		if (!valid) {
//			printf("target no segment in candidate\n");
			return OVERMINDIS;
		}
		total_min_dist += min_dis;
	}		

//	printf("Target has segment in candidate\n");
	for (int index = 0; index < m && total_min_dist <= thres_dis; index++) {
		const PointInfo& candidate_point = can_point_list[index];
		double min_dis;
		bool valid = GetMinDistance(grid_panel, candidate_point, target, &min_dis);
		if (!valid) {
//			printf("candidate no segment in target\n");
			return OVERMINDIS;
		}
		total_min_dist += min_dis;
	}

//	printf("candidate has segment in target\n");
	printf("total_min_dist %lf, thres_dis %lf\n", total_min_dist, thres_dis);
	if (total_min_dist > thres_dis) {
//		printf("total %lf, over thres_dis %lf\n", total_min_dist, thres_dis);
		return OVERMINDIS;
	}
	
	double sim = 1 - total_min_dist / ((double)(n + m) * DMAX); 
	return sim;
}

bool ExpansionVerify::GetMinDistance(GridPanel& grid_panel, const PointInfo& target_point, const Trajectory& candidate, double *min_dis) {
	const vector<pair<int, int>>& around_grid = target_point.neighbour_grid_;
	int tra_id = candidate.id();	
	int grid_size = around_grid.size();
	const pair<int, int>& point_grid = target_point.grid_index_;
	*min_dis = DBL_MAX;
	vector<pair<double, int>> min_dis_grid;
	double x_part = target_point.x_part;
	double y_part = target_point.y_part;
	for (int i = 0; i < grid_size; i++) {
		const pair<int, int>& grid_index = around_grid[i];
		if (grid_index.first == point_grid.first && grid_index.second == point_grid.second) {
			min_dis_grid.push_back(make_pair(0, i));
		} else if (grid_index.first == point_grid.first && grid_index.second == point_grid.second + 1) {
			min_dis_grid.push_back(make_pair(DISUNIT - y_part, i));
		} else if (grid_index.first == point_grid.first && grid_index.second == point_grid.second - 1) {
			min_dis_grid.push_back(make_pair(y_part, i));
		} else if (grid_index.first == point_grid.first - 1 && grid_index.second == point_grid.second) {
			min_dis_grid.push_back(make_pair(x_part, i));
		} else if (grid_index.first == point_grid.first - 1 && grid_index.second == point_grid.second - 1) {
			min_dis_grid.push_back(make_pair(sqrt(x_part * x_part + y_part * y_part), i));
		} else if (grid_index.first == point_grid.first - 1 && grid_index.second == point_grid.second + 1) {
			min_dis_grid.push_back(make_pair(sqrt(x_part * x_part + (DISUNIT - y_part) * (DISUNIT - y_part)), i));
		} else if (grid_index.first == point_grid.first + 1 && grid_index.second == point_grid.second) {
			min_dis_grid.push_back(make_pair(DISUNIT - x_part, i));
		} else if (grid_index.first == point_grid.first + 1 && grid_index.second == point_grid.second - 1) {
			min_dis_grid.push_back(make_pair(sqrt((DISUNIT - x_part ) * (DISUNIT - x_part) + y_part * y_part), i));
		} else if (grid_index.first == point_grid.first + 1 && grid_index.second == point_grid.second + 1) {
			min_dis_grid.push_back(make_pair(sqrt((DISUNIT - x_part) * ( DISUNIT - x_part) + (DISUNIT - y_part) * (DISUNIT - y_part)), i));
		}
	}		

	sort(min_dis_grid.begin(), min_dis_grid.end(), sort_by_dis);
	for(int i = 0; i < grid_size; i++) {
		const pair<double, int>& dis_grid = min_dis_grid[i];
		const pair<int, int>& grid_index = around_grid[dis_grid.second];
		Panel::segmentset& intersect_segments = grid_panel.panel().GetSegmentsInGrid(grid_index, tra_id);	
		if (intersect_segments.empty()) {
			continue;
		}
		Panel::segmentset::iterator end_itor = intersect_segments.end();
		for (Panel::segmentset::iterator itor = intersect_segments.begin(); itor != end_itor; ++itor) {
			const pair<int, int>& cur_seg = *itor;
			double cur_min_dis = GetMinDistance(target_point, candidate, cur_seg);
			if (cur_min_dis < *min_dis) {
				*min_dis = cur_min_dis;
			}
		}
		if (i != grid_size - 1 && *min_dis < min_dis_grid[i + 1].first) {
			break;
		}
	}
	return (*min_dis != DBL_MAX);
}

double ExpansionVerify::GetMinDistance(const PointInfo& target, const Trajectory& candidates, const pair<int, int>& cur_seg) {
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
