#include <algorithm>
#include "panel_insert_test.h"
#include "log.h"
#include "../Tra/traj_data.h"
#include "../Strategy/binary_strategy.h"
#include "../Strategy/special_point_strategy.h"
#include "../Strategy/end_point_strategy.h"
#include "../Strategy/end_ascommon_strategy.h"
#include "../Strategy/single_point_strategy.h"
#include "../Strategy/two_point_strategy.h"
#include "../Strategy/k_point_strategy.h"
#include "../Strategy/min_dis_strategy.h"
#include "../Strategy/correlation_strategy.h"
#include "../Strategy/all_point_strategy.h"
#include "../Verify/verify.h"
#include "../Verify/simple_verify.h"
#include "../Verify/early_verify.h"

using namespace std;

void Join(TrajData&, GridPanel&, Strategy& strategy, unordered_map<int, list<int>>& can_map, unordered_map<int, unordered_map<int, double>>& sim_map);

void Verify(GridPanel& grid_panel, BaseVerify& vrf_method, unordered_map<int, list<int>>& can_map, unordered_map<int, unordered_map<int, double>>& sim_map);

void FindGroundTruth(TrajData&, GridPanel&);
void FindGroundTruth(unordered_map<int, list<int>>& can_map, GridPanel&);

void OutputTruth(unordered_map<int, unordered_map<int, double>>& sim_map, GridPanel& grid_panel);

void StoreTrajs(TrajData&);

void ReadAndProcess();

bool order_by_size(const pair<int, int>& left, const pair<int, int>& rhs) {
		return left.second > rhs.second;
}

int main() {
	ReadAndProcess();
}

void ReadAndProcess() {
	TrajData traj_data;
	GridPanel grid_panel(BJMAP, WIDTH, HEIGHT, &traj_data); 

	clock_t read_cost;
	read_cost = clock();
	read_traj(traj_data);
	read_cost = clock() - read_cost;
	//int point_size =0 ;// grid_panel.PointSize();

	string readInfo = "read trajs: " + to_string((int)traj_data.trajs.size()) + ", cost time " + to_string((double)read_cost / CLOCKS_PER_SEC);
	Log::log(0, readInfo);
	printf("%s\n", readInfo.c_str());

	clock_t sort_cost = clock();
	sort(traj_data.traj_index_point_count.begin(), traj_data.traj_index_point_count.end(), order_by_size);
	sort_cost = clock() - sort_cost;
	string sortInfo = string("Sort needs ") + to_string((double)sort_cost / CLOCKS_PER_SEC); 
	Log::log(0, sortInfo);
	printf("%s\n", sortInfo.c_str());
	
	int traj_size = traj_data.trajs.size();
	for (int i = 0; i < traj_size; i++) {
		int index = traj_data.traj_index_point_count[i].first;
		traj_data.trajs[index].set_id(i + 1);
		traj_data.traj_id_index[i + 1] = index;	
	}

	unordered_map<int, list<int>> can_map;
	unordered_map<int, unordered_map<int, double>> sim_map;

	//BinaryStrategy binary_strategy(0, traj.point_list().size() - 1);	
	//SpecialPointStrategy strategy;
	//EndPointStrategy strategy;
	//EndAsCommonStrategy strategy;
//	SinglePointStrategy strategy;
//	TwoPointStrategy strategy;
//	KPointStrategy strategy(3);
//	MinDisStrategy strategy(3, SIMTHRESHOLD);
//	CorrelationStrategy strategy;
	AllPointStrategy strategy;
	string joinInfo = "GridSide: " + to_string(DISUNIT) + ", DMAX: " + to_string(DMAX) + ", threshold: " + to_string(SIMTHRESHOLD) + ", Strategy: " + strategy.name();
	Log::log(0, joinInfo);
	printf("%s\n", joinInfo.c_str());
	Join(traj_data, grid_panel, strategy, can_map, sim_map);
}


void StoreTrajs(TrajData& traj_data) {
	string dir = "trajfile/";
	vector<Trajectory>& trajs = traj_data.trajs;
	int size = trajs.size();
	for (int i = 0; i < size; i++) {
		const Trajectory& traj = trajs[i];
		int id = traj.id();
		string file_path = dir + to_string(id) + ".csv";
		output_traj(traj, file_path);
	}
}


/*
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

*/

/*
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
*/

/*
void FindGroundTruth(TrajData& traj_data, GridPanel& grid_panel) {
/ *	clock_t insert_cost = clock();
	grid_panel.InsertTrajectory(traj_data.trajs);
	insert_cost = clock() - insert_cost;
	double insert_time = insert_cost / CLOCKS_PER_SEC;
	int traj_size = traj_data.trajs.size();
	printf("Trajs count: %d, Insert cost: %lf, per traj : %lf\n", traj_size, insert_time, insert_time / traj_size);
* /
	printf("Begin output traj\n");
	Log::log(0, "Begin output traj");
	get_candidate_output(traj_data, grid_panel);
	Log::log(0, "End out traj");
}
*/
void Join(TrajData& traj_data, GridPanel& grid_panel, Strategy& strategy, unordered_map<int, list<int>>& can_map, unordered_map<int, unordered_map<int, double>>& sim_map) {	
	Log::log(0, "Begin Join\n");
	clock_t join_cost = clock();

	JoinAndCandidate(grid_panel, strategy, traj_data, can_map);

	join_cost = clock() - join_cost;
	double total_cost = join_cost / CLOCKS_PER_SEC;
	double per_cost = total_cost / traj_data.trajs.size();
	
	string joinInfo = "join cost: " + to_string(total_cost) + ", per traj needs " + to_string(per_cost);
	Log::log(0, joinInfo);
	printf("join cost: %lf, per traj needs %lf\n", total_cost, per_cost);

	SimpleVerify vrf_simple;
	EarlyVerify vrf_early; 	
	Verify(grid_panel, vrf_simple, can_map, sim_map);
	Verify(grid_panel, vrf_early, can_map, sim_map);
	
}

void Verify(GridPanel& grid_panel, BaseVerify& vrf_method, unordered_map<int, list<int>>& can_map, unordered_map<int, unordered_map<int, double>>& sim_map) {
	string begin_verify = "begin verify " + vrf_method.name();
	Log::log(0, begin_verify);
	printf("%s\n", begin_verify.c_str());
	clock_t sim_cost = clock();

	long long pair_count = VerifySim(grid_panel, vrf_method, can_map, sim_map);

	sim_cost = clock() - sim_cost;
	double sim_total = (double)sim_cost / CLOCKS_PER_SEC;
	double sim_per = sim_total / pair_count;
	
	string verifyInfo = "calculate sim time : " + to_string(sim_cost) + ", seconds : " + to_string(sim_total) + ", pair_count : " + to_string(pair_count) + ", per pair needs " + to_string(sim_per);
	Log::log(0, verifyInfo);
	printf("%s\n", verifyInfo.c_str());
}
