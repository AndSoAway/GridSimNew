#include <vector>
#include <climits>
#include <cmath>
#include "simple_verify.h"
#include "../Tra/trajectory.h"
#include "../Tra/sample_point.h"
#include "../Tra/utils.h"
#include "../Tra/config.h"

using namespace std;

double SimpleVerify::verify(const GridPanel* grid_panel, const Trajectory& target, const Trajectory& candidate, double threshold) {
	//double tr2can_min_dist = 0;
	//double can2tr_min_dist = 0;
	double total_min_dist = 0;
	
	const vector<PointInfo>& tr_point_list = target.point_list();
	int n = tr_point_list.size();
	const vector<PointInfo>& can_point_list = candidate.point_list(); 
	int m = can_point_list.size();

//	printf("n is %d\n", n);
	for (int index = 0; index < n; index++) {
		double min_dis = minDistance(tr_point_list.at(index), candidate);	      
		total_min_dist += min_dis;
//		printf("tar id %d, can id %d, min_dis %lf\n", target.id(), candidate.id(), min_dis);
		//tr2can_min_dist += min_dis;
	}

	for (int index = 0; index < m; index++) {
		double min_dis = minDistance(can_point_list.at(index), target);
		total_min_dist += min_dis;
	//	printf("can id %d, tar id %d, min_dis %lf\n", candidate.id(), target.id(), min_dis);
	//	can2tr_min_dist += min_dis;
	}
	
//	printf("DISUNIT is %d\n", DISUNIT);
	double sim = 1 - (total_min_dist) / ((double)(n + m) * DISUNIT);	
//	printf("get tr2can_min_dist %lf\n", tr2can_min_dist);
//	printf("get can2tr_min_dist %lf\n", can2tr_min_dist);
//	printf("DISUNIT %d, n %d, m %d\n", DISUNIT, n, m);
//	printf("get sim %lf\n", sim);
	return sim;
}
