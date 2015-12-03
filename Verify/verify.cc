#include <vector>
#include <climits>
#include <cmath>
#include "verify.h"
#include "../Tra/trajectory.h"
#include "../Tra/sample_point.h"
#include "../Tra/utils.h"

using namespace std;

double verify(const Trajectory& target, const Trajectory& candidate) {
	double tr2can_min_dist = 0;
	double can2tr_min_dist = 0;
	const vector<SamplePoint>& tr_point_list = target.point_list();
	int n = tr_point_list.size();
	printf("n is %d\n", n);
	for (int index = 0; index < n; index++) {
		double min_dis = minDistance(tr_point_list.at(index), candidate);	      
//		printf("tar id %d, can id %d, min_dis %lf\n", target.id(), candidate.id(), min_dis);
		tr2can_min_dist += min_dis;
	}

	const vector<SamplePoint>& can_point_list = candidate.point_list(); 
	int m = can_point_list.size();
	for (int index = 0; index < m; index++) {
		double min_dis = minDistance(can_point_list.at(index), target);
	//	printf("can id %d, tar id %d, min_dis %lf\n", candidate.id(), target.id(), min_dis);
		can2tr_min_dist += min_dis;
	}
	
	printf("DISUNIT is %d\n", DISUNIT);
	double sim = 1 - (tr2can_min_dist + can2tr_min_dist) / ((double)(n + m) * DISUNIT);	
//	printf("get tr2can_min_dist %lf\n", tr2can_min_dist);
//	printf("get can2tr_min_dist %lf\n", can2tr_min_dist);
//	printf("DISUNIT %d, n %d, m %d\n", DISUNIT, n, m);
//	printf("get sim %lf\n", sim);
	return sim;
}

double minDistance(const SamplePoint& point, const Trajectory& tra) {
	double min_dis = double(ULONG_MAX); 
	const vector<SamplePoint>& point_list = tra.point_list();
	for (int index = 1; index < point_list.size(); index++) {
		double cur_dis = minDistance(point, point_list.at(index), point_list.at(index - 1));
		if (cur_dis < min_dis) {
			min_dis = cur_dis;
		}
	} 
	return min_dis;
}

double minDistance(const SamplePoint& point, const SamplePoint& begin, const SamplePoint& end) {
	double a = disSample(point, begin);
	double b = disSample(point, end);	
	double c = disSample(begin, end);

	if (abs(a + b - c) < PRECISE) 
		return 0;

	if (c < PRECISE)
		return a;

	if (b * b >= a * a + c * c)
		return a;

	if (a * a >= b * b + c * c)
		return b;

	double cir = (a + b + c) / 2;
	double area = sqrt(cir * (cir - a) * (cir - b) * (cir - c));
  double res =  a * area / c;
	return res;
}
