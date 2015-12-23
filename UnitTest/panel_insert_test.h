#ifndef UNIT_TEST_PANEL_INSERT_TEST_H
#define UNIT_TEST_PANEL_INSERT_TEST_H
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <set>
#include <fstream>
#include <string>
#include "../Verify/verify.h"
#include "../Tra/config.h"
#include "../Grid/rectangle.h"
#include "../Tra/point.h"
#include "../Grid/grid_panel.h"
#include "../Tra/trajectory_helper.h"
#include "../Tra/trajectory.h"
#include "../Tra/traj_data.h"

#include "../Strategy/strategy.h"

void panel_insert_test(GridPanel&, TrajData&);

void file_read_speed();

void test_tra_id(const std::vector<SamplePoint>& trajs);

void read_traj(TrajData& traj_data);

void traj_archive(TrajData& traj_data, std::string file_name);

void traj_load(TrajData& traj_data, std::string file_name);
void output_traj(const Trajectory& traj, std::string& file_name);
//void get_candidate_output(TrajData& traj_data, GridPanel& grid_panel);

void filterFile();

void JoinAndCandidate(GridPanel& grid_panel, Strategy& strategy, TrajData& traj_data, std::unordered_map<int, std::list<int> >& can_map);

int VerifySim(GridPanel& grid_panel, std::unordered_map<int, std::list<int>>& can_map, std::unordered_map<int, std::unordered_map<int, double>>& sim_map);


int GetCandidate(GridPanel& grid_panel, Strategy& strategy, Trajectory& traj, std::unordered_map<int, std::list<int>>& can_map);	

//void DistSimplify();
void output_sim(std::unordered_map<int, std::unordered_map<int, double> >&, std::string& file);
#endif
