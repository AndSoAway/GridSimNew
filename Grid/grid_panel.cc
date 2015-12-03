#include <cstdio>
#include <vector>
#include <set>
#include "grid_panel.h"
#include "../Tra/trajectory_helper.h"
#include "../Tra/config.h"

using namespace std;

void GridPanel::InsertTrajectory(const vector<Trajectory>& trajs) {
   for(auto traj : trajs) {
      InsertTrajectory(traj);
   }
}

void GridPanel::InsertTrajectory(const Trajectory& traj) {
  if (trajs_.find(traj.id()) == trajs_.end()) {
    trajs_[traj.id()] = traj; 
    panel_.InsertTrajectory(traj);
  }
}

void GridPanel::FindCandidates(Strategy& strategy, const Trajectory& traj, double dis, list<int>& trajs) const {
	strategy.FindCandidateTrajs(this, traj, dis, trajs);
}

void GridPanel::PrintPanel() const {
  printf("Panel info: %s", panel_.info().c_str());
}

int GridPanel::GridSize() const {
  return panel_.GridSize();
}

int GridPanel::PointSize() const {
  return panel_.PointSize();
}

Trajectory& GridPanel::getTraj(int tra_id) {
	return trajs_.at(tra_id);	
}
