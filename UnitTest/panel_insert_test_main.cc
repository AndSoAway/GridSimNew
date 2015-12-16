#include "panel_insert_test.h"
#include "log.h"
#include "../Strategy/binary_strategy.h"
using namespace std;

void Join(TrajData&, GridPanel&, unordered_map<int, list<int>>& can_map, unordered_map<int, unordered_map<int, double>>& sim_map);

void FindGroundTruth(TrajData&, GridPanel&);
void FindGroundTruth(unordered_map<int, list<int>>& can_map, GridPanel&);

void OutputTruth(unordered_map<int, unordered_map<int, double>>& sim_map, GridPanel& grid_panel);

void StoreTrajs(TrajData&);

void ReadAndProcess();

int main() {
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

	string readInfo = "read trajs: " + to_string(traj_data.trajs.size()) + ", cost time " + to_string((double)read_cost / CLOCKS_PER_SEC);
	Log::log(0, readInfo);
	printf("read trajs: %ld, point size %d, cost time %lf\n", traj_data.trajs.size(), point_size, (double)read_cost / CLOCKS_PER_SEC);

//	StoreTrajs(traj_data);
	//FindGroundTruth(traj_data, grid_panel);
	unordered_map<int, list<int>> can_map;
	unordered_map<int, unordered_map<int, double>> sim_map;
	Join(traj_data, grid_panel, can_map, sim_map);
	FindGroundTruth(can_map, grid_panel);
	OutputTruth(sim_map, grid_panel);
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

void OutputTruth(unordered_map<int, unordered_map<int, double>>& sim_map, GridPanel& grid_panel) {
	string file_path = "res_csv/res/output_res_traj.csv";
	string file_ori = "res_csv/res/output_ori.csv";
	string file_dir = "res_csv/can";
	int sum = 0;
	for (unordered_map<int, unordered_map<int, double>>::iterator itor = sim_map.begin(); itor != sim_map.end() && sum <= OUTPUTCOUNT; itor++, sum++) {
		int test_id = itor->first;
		Trajectory& traj = grid_panel.getTraj(test_id);
		unordered_map<int, double>& res_map = itor->second;
		
		string file_dir_ = file_dir;
		
		file_dir_.insert(11, to_string(traj.id()));
		system(("mkdir " + file_dir_).c_str());

		string file_path_ = file_path;
		file_path_.insert(11, to_string(traj.id()));

		string file_ori_ = file_ori;
		file_ori_.insert(11, to_string(traj.id()));
		output_traj(traj, file_ori_);

		for(unordered_map<int, double>::iterator res_itor = res_map.begin(); res_itor != res_map.end(); res_itor++) {
			if (res_itor->second <= 0 || res_itor->second > 1)
				continue;

			int id = res_itor->first;
			string id_s = to_string(id);
			Trajectory& can_traj = grid_panel.getTraj(id);
			string output_file = file_path_;
			output_file.insert(output_file.size() - 4, to_string(id));
			output_traj(can_traj, output_file);	
		}
	}
}

void FindGroundTruth(unordered_map<int, list<int>>& can_map, GridPanel& grid_panel) {
 	string file_path = "traj_csv/can/output_can_traj.csv";
	string file_ori = "traj_csv/can/output_ori.csv";
	string file_dir = "traj_csv/can";
	int sum = 0;
	for(unordered_map<int, list<int>>::iterator test_itor = can_map.begin(); test_itor != can_map.end() && sum <= OUTPUTCOUNT; test_itor++, sum++) {
		int test_id = test_itor->first;
		Trajectory& traj = grid_panel.getTraj(test_id);
		list<int>& can_trajs = test_itor->second;
		printf("traj id %d, candidate %ld\n", test_id, can_trajs.size());
		string file_dir_ = file_dir;

		file_dir_.insert(12, to_string(traj.id()));
		system(("mkdir " + file_dir_).c_str());

		string file_path_ = file_path;
		file_path_.insert(12, to_string(traj.id()));

		string file_ori_ = file_ori;
		file_ori_.insert(12, to_string(traj.id()));
		output_traj(traj, file_ori_);
		
		for (list<int>::iterator itor = can_trajs.begin(); itor != can_trajs.end(); itor++) {
			int id = *itor;
			string id_s = to_string(id);
			Trajectory& can_traj = grid_panel.getTraj(id);
			string output_file = file_path_;
			output_file.insert(output_file.size() - 4, to_string(id));
			output_traj(can_traj, output_file);
		}
	}
}

void FindGroundTruth(TrajData& traj_data, GridPanel& grid_panel) {
/*	clock_t insert_cost = clock();
	grid_panel.InsertTrajectory(traj_data.trajs);
	insert_cost = clock() - insert_cost;
	double insert_time = insert_cost / CLOCKS_PER_SEC;
	int traj_size = traj_data.trajs.size();
	printf("Trajs count: %d, Insert cost: %lf, per traj : %lf\n", traj_size, insert_time, insert_time / traj_size);
*/
	printf("Begin output traj\n");
	Log::log(0, "Begin output traj");
	get_candidate_output(traj_data, grid_panel);
	Log::log(0, "End out traj");
}

void Join(TrajData& traj_data, GridPanel& grid_panel, unordered_map<int, list<int>>& can_map, unordered_map<int, unordered_map<int, double>>& sim_map) {	
	Log::log(0, "Begin Join\n");
	clock_t join_cost = clock();
	JoinAndCandidate(grid_panel, traj_data.trajs, can_map);
	join_cost = clock() - join_cost;
	double total_cost = join_cost / CLOCKS_PER_SEC;
	double per_cost = total_cost / traj_data.trajs.size();
	
	string joinInfo = "join cost: " + to_string(total_cost) + ", per traj needs " + to_string(per_cost);
	Log::log(0, joinInfo);
	printf("join cost: %lf, per traj needs %lf\n", total_cost, per_cost);

	Log::log(0, "begin verify\n");
	clock_t sim_cost = clock();
	int pair_count = VerifySim(grid_panel, can_map, sim_map);
	sim_cost = clock() - sim_cost;
	double sim_total = (double)sim_cost / CLOCKS_PER_SEC;
	double sim_per = sim_total / pair_count;
	
	string verifyInfo = "calculate sim time : " + to_string(sim_cost) + ", seconds : " + to_string(sim_total) + ", pair_count : " + to_string(pair_count) + ", per pair needs " + to_string(sim_per);
	Log::log(0, verifyInfo);
	printf("calculate sim : %ld, seconds: %lf, pair_count: %d, per pair needs %lf\n", sim_cost, sim_total, pair_count, sim_per);

	Log::log(0, "begin output res of sim");
	string file_path = "sim_res.csv";
	output_sim(sim_map, file_path);
}

