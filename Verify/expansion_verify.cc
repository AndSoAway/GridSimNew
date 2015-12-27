#include "expansion_verify.h"

using namespace std;

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

bool SignatureVerify::GetMinDistance(GridPanel& grid_panel, const Trajectory& target_point, const Trajectory& candidate, double *min_dis) {
	
}
