#include "single_point_strategy.h"
#include "../UnitTest/log.h"
#include <utility>
#include <algorithm>
#include "../Tra/utils.h"

using namespace std;

inline double TOMS(clock_t x) {
	return   ((((double)x)/CLOCKS_PER_SEC) * 1000);
}

void SinglePointStrategy::FindCandidateTrajs(const GridPanel* grid_panel, const Trajectory& traj, double dis, list<int>& candidates) {
	const vector<PointInfo>& point_list = traj.point_list();
	int point_size = point_list.size();

//	clock_t total = clock();

//	clock_t get_cost = clock();
//	vector<pair<int, int>> point_grid;
//	for (int i = 0; i < point_size; i++) {
//		//const SamplePoint& point = point_list[i];
 //		clock_t get_cost = clock();
//		//int traj_size = GetTotalTrajCount(grid_panel, point, dis, false); 
//		int traj_size = grid_panel->panel().GetAroundTrajCount(point_list[i].grid_index_);
//		get_cost = clock() - get_cost;
//		string get_size = "Get One needs " + to_string(TOMS(get_cost));
/*		Log::log(1, get_size);
		point_grid.push_back(make_pair(i, traj_size));
	}
	get_cost = clock() - get_cost;
	string get_size = "Get "+ to_string(point_size) + " Size needs " + to_string(TOMS(get_cost));
	Log::log(1, get_size);
	
	clock_t sort_cost = clock();
	sort(point_grid.begin(), point_grid.end(), order_by_count);
	sort_cost = clock() - sort_cost;
	string sortInfo = "Sort " + to_string(point_size + 1) + " needs: " + to_string(TOMS(sort_cost));
	Log::log(1, sortInfo);


    	clock_t get_can = clock();
	int chose_index = point_grid[0].first;
*/
	int chose_index = (point_size - 1) / 2;
	const PointInfo& chose_point = point_list[chose_index];
	pair<int, int> grid_index = grid_panel->panel().GetGrid(chose_point);
	candidates = grid_panel->panel().GetTrajsAroundGrid(grid_index);
//	int can_size = candidates.size();
//	printf("tra %d, candidates size %d\n", traj.id(), can_size);
//	list<int>::iterator end = candidates.end();

//	for (list<int>::iterator itor = candidates.begin(); itor != end; ++itor) {
//		printf("%dth candidates\n", *itor);
//	}
//   	get_can = clock() - get_can;
//	int can_size = candidates.size();
//    	string getCanInfo = "Can " + to_string(can_size) + " time: " + to_string(TOMS(get_can)) + ", "; 
// 	Log::log(1, getCanInfo);

//	total = clock() - total;
//	string total_time = "Get can all time " + to_string(TOMS(total));
//	Log::log(1, total_time);
}
