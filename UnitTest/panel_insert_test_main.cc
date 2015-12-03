#include "panel_insert_test.h"
using namespace std;

int main() {
	TrajData traj_data;
	GridPanel grid_panel(BJMAP, WIDTH, HEIGHT); 


	clock_t read_cost;
	read_cost = clock();
	read_traj(traj_data);
	read_cost = clock() - read_cost;
	printf("read trajs: %ld, cost time %lf\n", traj_data.trajs.size(), (double)read_cost / CLOCKS_PER_SEC);
	
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
	double sim_total = sim_cost / CLOCKS_PER_SEC;
	double sim_per = sim_total / pair_count;
	printf("calculate sim : %lf, per pair needs %lf\n", sim_total, sim_per);
}
