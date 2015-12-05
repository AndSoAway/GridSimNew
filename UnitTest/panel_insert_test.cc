#include "panel_insert_test.h"
#include "file_path.h"
#include "../Strategy/binary_strategy.h"
#include "../Strategy/special_point_strategy.h"

using namespace std;

#define POINT_NUM 20
#define MAXTRAJCOUNT 10

int VerifySim(GridPanel& grid_panel, unordered_map<int, list<int> >& can_map, unordered_map<int, unordered_map<int, double>>& sim_map) {
	int pair_count = 0;
	for(unordered_map<int, list<int> >::iterator itor = can_map.begin(); itor != can_map.end(); itor++) {
		int first_id = itor->first;
		list<int>& can_list = itor->second;
		for (list<int>::iterator l_itor = can_list.begin(); l_itor != can_list.end(); l_itor++) {
			int second_id = *l_itor;
			Trajectory& first_traj = grid_panel.getTraj(first_id);
			Trajectory& second_traj = grid_panel.getTraj(second_id);
			double sim = verify(first_traj, second_traj);
			sim_map[first_id][second_id] = sim;
			pair_count++;
		}
	}
	return pair_count;
}

void JoinAndCandidate(GridPanel& grid_panel, const vector<Trajectory>& trajs, unordered_map<int, list<int> >& can_map) {
	for(auto traj : trajs) {
		GetCandidate(grid_panel, traj, can_map);
		grid_panel.InsertTrajectory(traj);
	}
}

void GetCandidate(GridPanel& grid_panel, const Trajectory& traj, unordered_map<int, list<int>> can_map) {	
	list<int> candidates;
	int id = traj.id();
	//BinaryStrategy binary_strategy(0, traj.point_list().size() - 1);	
	SpecialPointStrategy special_point_strategy(0, traj.point_list().size() - 1);
	grid_panel.FindCandidates(special_point_strategy, traj, DISUNIT, candidates);
	
	if (can_map.find(id) == can_map.end()) {
		can_map[id] = candidates;
	}
	printf("Test id %d, candidates size %ld, traj size %d\n", id, candidates.size(), grid_panel.TrajSize());
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

/*
void get_candidate_output(TrajData& traj_data, GridPanel& grid_panel) {
	BinaryStrategy binary_strategy(0, traj_data.trajs.size() - 1);
  int sum = 0;
 	string file_path = "traj_csv/can/output_can_traj.csv";
	string file_ori = "traj_csv/can/output_ori.csv";
	string file_dir = "traj_csv/can";
  while (binary_strategy.HasNext() && sum < 100) {
		int cur = binary_strategy.Next();
		Trajectory traj = traj_data.trajs[cur];
		if (traj.point_list().size() < POINT_NUM)
			continue;
		set<int> can_trajs;
	//	grid_panel.BinaryFilter(traj, can_trajs);
//		grid_panel.TopThreeJoin(traj, can_trajs);
		grid_panel.BinaryFilter(traj, can_trajs);
		if (can_trajs.empty() || can_trajs.size() >= MAXTRAJCOUNT) {
			continue;
		}
		sum++;
		printf("sum %d, traj id %d, candidate %ld\n", sum, traj.id(), can_trajs.size());
		string file_dir_ = file_dir;
		file_dir_.insert(12, to_string(traj.id()));
		system(("mkdir " + file_dir_).c_str());
		string file_path_ = file_path;
		file_path_.insert(12, to_string(traj.id()));
		string file_ori_ = file_ori;
		file_ori_.insert(12, to_string(traj.id()));
		output_traj(traj, file_ori_);
		for (set<int>::iterator itor = can_trajs.begin(); itor != can_trajs.end(); itor++) {
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
void read_traj(TrajData& traj_data) {
  TrajectoryHelper* trajHelper = TrajectoryHelper::GetHelperInstance();
  for (int i = 0;  i < FILE_NUM; i++) {
		FILE* file = fopen(file_paths[i], "rb");
  //for (int i = 0; i < 2; i++) {
	//	FILE* file = fopen(test_files[i], "rb");
		//FILE* file = fopen(filter_file_paths[i], "rb");
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
