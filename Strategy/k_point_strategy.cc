#include "k_point_strategy.h"
#include "../UnitTest/log.h"
#include <utility>
#include <algorithm>

using namespace std;

void  KPointStrategy:: FindCandidateTrajs(const GridPanel* grid_panel, const Trajectory& traj, double dis, list<int>& candidates) {
	const vector<SamplePoint>& point_list = traj.point_list();
	int point_size = point_list.size();
	
//	clock_t total = clock();

//	clock_t get_cost = clock();
	vector<pair<int, int>> point_grid;
	for (int i = 0; i < point_size; i++) {
		const SamplePoint& point = point_list[i];
		//clock_t get_cost = clock();
		int traj_size = GetTotalTrajCount(grid_panel, point, dis, false); 
		//get_cost = clock() - get_cost;
		//string get_size = "Get One needs " + to_string(TOMS(get_cost));
		//Log::log(1, get_size);
		point_grid.push_back(make_pair(i, traj_size));
	}
//	get_cost = clock() - get_cost;
//	string get_size = "Get "+ to_string(point_size) + " Size needs " + to_string(TOMS(get_cost));
//	Log::log(1, get_size);

	sort(point_grid.begin(), point_grid.end(), order_by_count);

	int chose_first = point_grid[0].first;
	const SamplePoint& first_point = point_list[chose_first];
	GetCandidateTrajs(grid_panel, first_point, dis, candidates);
	
	for (int index = 1; index < k_ && index < point_size; index++) {
		int chose_index = point_grid[index].first;
		const SamplePoint& chose_point = point_list[chose_index];
		list<int> sec_can;
		GetCandidateTrajs(grid_panel, chose_point, dis, sec_can);

		list<int> res;
		TrajMergeJoin(candidates, sec_can, res);
		candidates.swap(res);
	}
}

