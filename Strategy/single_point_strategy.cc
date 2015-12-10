#include "single_point_strategy.h"

void SinglePointStrategy::GetCandidateTrajs(const GridPanel* grid_panel, const Trajectory& traj, double dis, list<int>& candidates) {
	const vector<SamplePoint>& point_list = traj.point_list();
	int point_size = point_list.size();

	srand((unsigned)time((long*)0));
	int chose_index = rand() % point_size;
	
	const SamplePoint& chose_point = point_list[chose_index];

	if (chose_index == 0 || chose_index == point_size - 1) {
		GetCandidateTrajs(grid_panel, chose_point, dis, candidates, true);
	} else {
		GetCandidateTrajs(grid_panel, chose_point, dis, candidates, false);
	}
}
