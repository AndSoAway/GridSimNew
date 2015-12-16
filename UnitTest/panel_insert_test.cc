#include "panel_insert_test.h"
#include "file_path.h"
#include "log.h"
#include "../Strategy/binary_strategy.h"
#include "../Strategy/special_point_strategy.h"
#include "../Strategy/end_point_strategy.h"
#include "../Strategy/end_ascommon_strategy.h"
#include "../Strategy/single_point_strategy.h"
#include "../Strategy/two_point_strategy.h"
#include "../Strategy/k_point_strategy.h"

using namespace std;

#define POINT_NUM 20
#define MAXTRAJCOUNT 10
#define MIN_DIST 20

int VerifySim(GridPanel& grid_panel, unordered_map<int, list<int> >& can_map, unordered_map<int, unordered_map<int, double>>& sim_map) {
	int pair_count = 0;
	int success_pair_count = 0;
	clock_t verify_clock  = 0;
	time_t verify_begin = time(NULL);;
	time_t verify_end = time(NULL);
	for(unordered_map<int, list<int> >::iterator itor = can_map.begin(); itor != can_map.end(); itor++) {
		int first_id = itor->first;
		list<int>& can_list = itor->second;
		for (list<int>::iterator l_itor = can_list.begin(); l_itor != can_list.end(); l_itor++) {
			int second_id = *l_itor;
			Trajectory& first_traj = grid_panel.getTraj(first_id);
			Trajectory& second_traj = grid_panel.getTraj(second_id);

			clock_t verify_cur = clock();
			double sim = verify(first_traj, second_traj);
			verify_cur = clock() - verify_cur;
			verify_clock += verify_cur;
			
			sim_map[first_id][second_id] = sim;
			if (sim >= 0 && sim <= 1.0)
				success_pair_count++;
			pair_count ++;
			if (pair_count % 10000 == 0) {
				verify_end = time(NULL);
				string verifyInfo = "Verfiy tra count " + to_string(pair_count) + ", verify clock: " + to_string((double)verify_clock / CLOCKS_PER_SEC) + ", success_pair_count: " + to_string(success_pair_count) + ", time interval: " + to_string(verify_end  - verify_begin);
				Log::log(0, verifyInfo);
				printf("%s\n", verifyInfo.c_str());
				verify_clock = 0;
				verify_begin = time(NULL);		
			}
		}
	}

	string verifyInfo = "total pair count " + to_string(pair_count) + ", success pair count " + to_string(success_pair_count);
	Log::log(0, verifyInfo);
	printf("total pair count %d, success pair count %d\n", pair_count, success_pair_count);
	return pair_count;
}

void JoinAndCandidate(GridPanel& grid_panel, const vector<Trajectory>& trajs, unordered_map<int, list<int> >& can_map) {
	int count = 0;
	clock_t query_time = 0;
	clock_t insert_time = 0;	
	int total_can_pair = 0; 
	time_t begin_ts = time(NULL);
	time_t end_ts = time(NULL);
	for(auto traj : trajs) {
		count ++;
		if (count % 10000 == 0) {
			end_ts = time(NULL);
				
			string processInfo = "Joined tra count " + to_string(count) + ", query_time: " + to_string((double)query_time / CLOCKS_PER_SEC) + ", insert_time: " + to_string((double)insert_time / CLOCKS_PER_SEC) + ", time_interval "+ to_string(end_ts - begin_ts) + ", new pair count: " + to_string(total_can_pair);;
			Log::log(0, processInfo);	
			printf("Joined tra count %d\n", count);
			query_time = 0;
			insert_time = 0;
			total_can_pair = 0;
			begin_ts = time(NULL);
		}	
		clock_t tmp_query = clock();
		int cur_size = GetCandidate(grid_panel, traj, can_map);
		tmp_query = clock() - tmp_query;
		query_time += tmp_query;

		total_can_pair += cur_size;

		clock_t tmp_insert = clock();
		grid_panel.InsertTrajectory(traj);
		tmp_insert = clock() - tmp_insert;
		insert_time += tmp_insert;
	}
}

int GetCandidate(GridPanel& grid_panel, const Trajectory& traj, unordered_map<int, list<int>>& can_map) {	
	list<int> candidates;
	int id = traj.id();
	//printf("Test traj id %d, point size %ld, Dmax %d\n", id, traj.point_list().size(), DISUNIT);
	//BinaryStrategy binary_strategy(0, traj.point_list().size() - 1);	
	SpecialPointStrategy strategy;
	//EndPointStrategy strategy;
	//EndAsCommonStrategy strategy;
	//SinglePointStrategy strategy;
	//TwoPointStrategy strategy;
	//KPointStrategy strategy(3);
	grid_panel.FindCandidates(strategy, traj, DMAX, candidates);
	//if (!candidates.empty())
		//printf("id %d can size %ld\n", id, candidates.size());
	if (can_map.find(id) == can_map.end()) {
		can_map[id] = candidates;
	}
	return candidates.size();
//	printf("Test id %d, candidates size %ld, traj size %d\n", id, candidates.size(), grid_panel.TrajSize());
/*
		list<int> cur_can = can_map[id];
		list<int>::iterator itor = cur_can.begin();
		bool contain = false;
		while ( itor != cur_can.end()) {
			if (*itor == id) {
				contain = true;
				break;
			} else if(*itor > id) {
				break;
			}
		}
		if (!contain) {
			cur_can.insert(itor, id);
		}
*/
}

void filterFile() {
	for (int i = 0; i < FILE_NUM; i++) {
		string orifile = file_paths[i];
		string output = orifile;
		output.insert(orifile.size() - 4, "filter");
		FILE* file = fopen(file_paths[i], "rb");
		filterLoaded(file, output);
		fclose(file);
	}
}


void get_candidate_output(TrajData& traj_data, GridPanel& grid_panel) {
	//BinaryStrategy binary_strategy(0, traj_data.trajs.size() - 1);
	int size = traj_data.trajs.size();
  int sum = 0;
 	string file_path = "traj_csv/can/output_can_traj.csv";
	string file_ori = "traj_csv/can/output_ori.csv";
	string file_dir = "traj_csv/can";
  for (int i = 0; i < size; i++) {// && sum < 100) {
		//int cur = binary_strategy.Next();
		Trajectory traj = traj_data.trajs[i];
		//if (traj.point_list().size() < POINT_NUM)
	//		continue;
		list<int> can_trajs;
	//	SpecialPointStrategy strategy(0, traj.point_list().size() - 1);
		//EndPointStrategy strategy;
		SinglePointStrategy strategy;
		grid_panel.FindCandidates(strategy, traj, DISUNIT, can_trajs);
		can_trajs.remove(traj.id());
		if (can_trajs.empty()) {// || can_trajs.size() >= MAXTRAJCOUNT) {
			continue;
		}
		sum++;
//		printf("sum %d, traj id %d, candidate %ld\n", sum, traj.id(), can_trajs.size());
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

void read_traj(TrajData& traj_data) {
  TrajectoryHelper* trajHelper = TrajectoryHelper::GetHelperInstance();
  /*for (int i = 0;  i < FILE_NUM; i++) {
		FILE* file = fopen(file_paths[i], "rb");*/

//  for (int i = 0; i < 2; i++) {
//		FILE* file = fopen(test_files[i], "rb");

  for (int i = 0;  i < FILE_NUM; i++) {
		FILE* file = fopen(filter_file_paths[i], "rb");
		printf("read %s\n", filter_file_paths[i]);
		trajHelper->ExtractTrajectory(file, traj_data.trajs);
		fclose(file);
	}
}

void panel_insert_test(GridPanel& grid_panel, TrajData& traj_data) {
	time_t insert_begin, insert_end;
	time(&insert_begin);
  grid_panel.InsertTrajectory(traj_data.trajs);
  time(&insert_end);
	printf("insert cost time : %lf\n", difftime(insert_end, insert_begin));
}

void output_traj(const Trajectory& traj, std::string& file_name) {
//	printf("output traj %s\n", file_name.c_str());
	FILE* output = fopen(file_name.c_str(), "wb");
	if (output == NULL)
		printf("output cannot open\n");
  const vector<SamplePoint>& points = traj.point_list();
  for (vector<SamplePoint>::const_iterator citor = points.cbegin(); citor != points.cend(); ++ citor) {
		fputs(citor->line().c_str(), output);
	}
  fclose(output);
}

void file_read_speed() {
	char line[100];
	time_t read_begin, read_end;
	time(&read_begin);
  for (int i = 0; i < FILE_NUM; i++) {
	  time_t file_begin, file_end;
		time(&file_begin);
	  FILE* file = fopen(file_paths[i], "rb");
		int num = 0;
		while (fgets(line, 100, file) != NULL) {
		  num++;
		}
		time(&file_end);
		printf("read file %s, cost time%lf\n", file_paths[i], difftime(file_end, file_begin));
		fclose(file);
	}
	time(&read_end);
	printf("total cost time : %lf\n", difftime(read_end, read_begin));
}

void traj_archive(TrajData& traj_data, string file_name) {
  ofstream ofs(file_name.c_str());
  oarchive oa(ofs);
  oa << traj_data;
	ofs.close();
}

void traj_load(TrajData& traj_data, string file_name) {
  ifstream ifs(file_name.c_str(), ios::binary);
  iarchive ia(ifs);
  ia >> traj_data;
  ifs.close();
}

void DistSimplify() {
  for (int i = 0; i < FILE_NUM; i++) {
			string file_ori = filter_file_paths[i];
			file_ori.insert(0, "sim");
      FILE *file = fopen(filter_file_paths[i], "rb");
			FILE *out_file = fopen(file_ori.c_str(), "wb");
			SamplePoint pre, aft;
      fscanfPoint(file, pre);
			int sum = 0;
			int smplfy = 0;
			fputs(pre.line().c_str(), out_file);
			while (fscanfPoint(file, aft) > 0) {
				double pre_x = pre.point().x() / ENLARGE;
				double pre_y = pre.point().y() / ENLARGE;
				double aft_x = aft.point().x() / ENLARGE;
 				double aft_y = aft.point().y() / ENLARGE;
				double dist = distHaversineRAD(pre_x, pre_y, aft_x, aft_y);
				if (dist > 20) {
					fputs(aft.line().c_str(), out_file);
					//printf("dis %lf\n", dist);
					pre = aft;
				} else {
					printf("dis %lf\n", dist);
					smplfy++;
				}
				sum++;
			}
			printf("total %d, sim %d\n", sum , smplfy);
			fclose(file);
			fclose(out_file);
  }
}

void output_sim(unordered_map<int, unordered_map<int, double>>& sim_map, string& file_path) {
	FILE *file = fopen(file_path.c_str(), "wb");
	for(unordered_map<int, unordered_map<int, double>>::iterator itor = sim_map.begin(); itor != sim_map.end(); itor++) {
		string res = "{" + to_string(itor->first) + ": {";
		unordered_map<int, double>& res_map = itor->second;
		int can_size = res_map.size();
		res = res + "can_size : " + to_string(can_size) + ","; 
		res = res + "res:{";
		for(unordered_map<int, double>::iterator u_itor = res_map.begin(); u_itor != res_map.end(); u_itor++) {
			res = res + to_string(u_itor->first) + ":" +  to_string(u_itor->second) + ",";
		}
		res += "}}}\n";
		fputs(res.c_str(), file);
	}
}
