#include "panel_insert_test.h"
#include "../Strategy/binary_strategy.h"
using namespace std;

void Join(TrajData&, GridPanel&);

void FindGroundTruth(TrajData&, GridPanel&);

void StoreTrajs(TrajData&);

void ReadAndProcess();

int main() {
//	DistSimplify();
	ReadAndProcess();
}

void ReadAndProcess() {
	TrajData traj_data;
	GridPanel grid_panel(BJMAP, WIDTH, HEIGHT); 

	clock_t read_cost;
	read_cost = clock();
	read_traj(traj_data);
	read_cost = clock() - read_cost;
	int point_size = grid_panel.PointSize();
	printf("read trajs: %ld, point size %d, cost time %lf\n", traj_data.trajs.size(), point_size, (double)read_cost / CLOCKS_PER_SEC);

//	StoreTrajs(traj_data);
	//FindGroundTruth(traj_data, grid_panel);
	Join(traj_data, grid_panel);
	FindGroundTruth(traj_data, grid_panel);
}

void StoreTrajs(TrajData& traj_data) {
	string dir = "trajfile/";
	vector<Trajectory>& trajs = traj_data.trajs;
	for (vector<Trajectory>::iterator itor = trajs.begin(); itor != trajs.end(); itor++) {
		int id = itor->id();
		string file_path = dir + to_string(id) + ".csv";
		output_traj(*itor, file_path);
	}
}

void FindGroundTruth(TrajData& traj_data, GridPanel& grid_panel) {
	clock_t insert_cost = clock();
	grid_panel.InsertTrajectory(traj_data.trajs);
	insert_cost = clock() - insert_cost;
	double insert_time = insert_cost / CLOCKS_PER_SEC;
	int traj_size = traj_data.trajs.size();
	printf("Trajs count: %d, Insert cost: %lf, per traj : %lf\n", traj_size, insert_time, insert_time / traj_size);
	get_candidate_output(traj_data, grid_panel);
}

void Join(TrajData& traj_data, GridPanel& grid_panel) {	
	printf("Begin Join\n");
	unordered_map<int, list<int> > can_map;
	clock_t join_cost = clock();
	JoinAndCandidate(grid_panel, traj_data.trajs, can_map);
	join_cost = clock() - join_cost;
	double total_cost = join_cost / CLOCKS_PER_SEC;
	double per_cost = total_cost / traj_data.trajs.size();
	printf("join cost: %lf, per traj needs %lf\n", total_cost, per_cost);

	unordered_map<int, unordered_map<int, double> > sim_map;
	clock_t sim_cost = clock();
	int pair_count = VerifySim(grid_panel, can_map, sim_map);
	sim_cost = clock() - sim_cost;
	double sim_total = (double)sim_cost / CLOCKS_PER_SEC;
	double sim_per = sim_total / pair_count;
	printf("calculate sim : %ld, seconds: %lf, pair_count: %d, per pair needs %lf\n", sim_cost, sim_total, pair_count, sim_per);

	string file_path = "sim_res.csv";
	output_sim(sim_map, file_path);
}

