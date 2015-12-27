#include <cfloat>
#include "signature_verify.h"
#include "../Tra/utils.h"
#include "../Grid/panel.h"

using namespace std;
double SignatureVerify::verify(GridPanel& grid_panel, const Trajectory& target, const Trajectory& candidate, double threshold) {
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
//	printf("total_min_dist %lf, thres_dis %lf\n", total_min_dist, thres_dis);
	if (total_min_dist > thres_dis) {
//		printf("total %lf, over thres_dis %lf\n", total_min_dist, thres_dis);
		return OVERMINDIS;
	}
	
	double sim = 1 - total_min_dist / ((double)(n + m) * DMAX); 
	return sim;
}
	
bool SignatureVerify::GetMinDistance(GridPanel& grid_panel, const PointInfo& target_point, const Trajectory& candidate, double *min_dis) {
	const vector<pair<int, int>>& around_grid = target_point.neighbour_grid_;
	int tra_id = candidate.id();
	int grid_size = around_grid.size();
	*min_dis = DBL_MAX;
//	printf("target pair<%d, %d>, grid_size %d\n", target_point.grid_index_.first, target_point.grid_index_.second, grid_size);
	for (int i = 0; i < grid_size; i++) {
		const pair<int, int>& grid_index = around_grid[i];
		Panel::segmentset& intersect_segments = grid_panel.panel().GetSegmentsInGrid(grid_index, tra_id);
//		printf("target pair<%d, %d> ", grid_index.first, grid_index.second);
		if (intersect_segments.empty()) {
//			printf("segments is empty\n");
			continue;
		} 
//		printf("segments is not empyty\n");
		Panel::segmentset::iterator end_itor = intersect_segments.end();
		for (Panel::segmentset::iterator itor = intersect_segments.begin(); itor != end_itor; ++itor) {
			const pair<int, int>& cur_seg = *itor;
			double cur_min_dis = GetMinDistance(target_point, candidate, cur_seg); 
			if (cur_min_dis < *min_dis) {
				*min_dis = cur_min_dis;
			}
		}	
	}
//	printf("DBL_MAX is %lf, min_dis is %lf\n", DBL_MAX, *min_dis);
	return (*min_dis != DBL_MAX);
}
	
double SignatureVerify::GetMinDistance(const PointInfo& target, const Trajectory& candidates, const pair<int, int>& cur_seg) {
	const vector<PointInfo>& point_list = candidates.point_list();
	
	int begin = cur_seg.first;
	int end = cur_seg.second;
	int size = point_list.size();
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
