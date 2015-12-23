#include <cstdio>
#include <vector>
#include <set>
#include "grid_panel.h"
#include "../Tra/trajectory_helper.h"
#include "../Tra/config.h"

using namespace std;

void GridPanel::InsertTrajectory(const vector<Trajectory>& trajs) {
	int count = 0;

	int traj_size = trajs.size();
	for(int i = 0;  i < traj_size; i++) {
		InsertTrajectory(trajs[i]);
		count++;
	}
}

void GridPanel::InsertTrajectory(const Trajectory& traj) {
/*	if (trajs_.find(traj.id()) == trajs_.end()) {
		trajs_[traj.id()] = traj; 
		panel_.InsertTrajectory(traj);
	}
*/
	panel_.InsertTrajectory(traj);
}

void GridPanel::FindCandidates(Strategy& strategy, Trajectory& traj, double dis, list<int>& trajs) const {
	strategy.FindCandidateTrajs(this, traj, dis, trajs);
}

/*
void GridPanel::PrintPanel() const {
	printf("Panel info: %s", panel_.info().c_str());
}
*/
/*
int GridPanel::GridSize() const {
	return panel_.GridSize();
}

int GridPanel::PointSize() const {
	return panel_.PointSize();
}
*/
const Trajectory& GridPanel::getTraj(int tra_id) const {
	int index = p_traj_data_->traj_id_index[tra_id];
	return p_traj_data_->trajs[index];	
}

Trajectory& GridPanel::getTraj(int tra_id) {
	int index = p_traj_data_->traj_id_index[tra_id];
	return p_traj_data_->trajs[index];	
}
