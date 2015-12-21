#ifndef GRID_GRID_PANEL_H
#define GRID_GRID_PANEL_H 1

#include <vector>
#include <unordered_map>
#include <set>

#include "panel.h"
#include "rectangle.h"
#include "../Tra/trajectory.h"
#include "../Strategy/strategy.h"

class Strategy;

class GridPanel {
public:
  GridPanel() { }

  GridPanel(const Rectangle& rec, double width, double height) {
    panel_ = Panel(rec, width, height);
  }

  void InsertTrajectory(const std::vector<Trajectory>& trajs);

  void InsertTrajectory(const Trajectory& traj);

  void FindCandidates(Strategy& strategy, Trajectory& traj, double dis, std::list<int>& trajs) const;

  void GetPointInfo(PointInfo& point_info, double dis) const { panel_.GetPointInfo(point_info, dis);}

  int GridSize() const;
  
  int PointSize() const;

  void PrintPanel() const ;

  int TrajSize() const { return trajs_.size(); }

  const Trajectory& getTraj(int tra_id) const;
	
  Trajectory& getTraj(int tra_id);

  const Panel& panel() const { return panel_; }
  Panel& panel() { return panel_; }

private:
  void traj_filter(std::set<int>& father_trajs, const std::set<int>& child_trajs) const;

  Panel panel_;
  std::unordered_map<int, Trajectory> trajs_;
};
#endif
