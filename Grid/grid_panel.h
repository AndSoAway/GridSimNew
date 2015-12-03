#ifndef GRID_GRID_PANEL_H
#define GRID_GRID_PANEL_H 1

#include <vector>
#include <unordered_map>
#include <set>

#include "panel.h"
#include "rectangle.h"

#include "../Tra/trajectory.h"

class GridPanel {
public:
  GridPanel() { }

  GridPanel(const Rectangle& rec, double width, double height) {
    panel_ = Panel(rec, width, height);
  }

  void InsertTrajectory(const std::vector<Trajectory>& trajs);

  void InsertTrajectory(const Trajectory& traj);

  void GetCandidateTrajs(const SamplePoint& point, double dis, std::set<int>& candidates) const;  

  int GridSize() const;
  
  int PointSize() const;

  void PrintPanel() const ;

  void BinaryFilter(const Trajectory& traj, std::set<int>& trajs) const;

	Trajectory& getTraj(int tra_id);

private:

  void traj_filter(std::set<int>& father_trajs, const std::set<int>& child_trajs) const;

  Panel panel_;
  std::unordered_map<int, Trajectory> trajs_;
};


class BinaryStrategy {
public:
  BinaryStrategy(int begin=0, int end=0): begin_(begin), end_(end), cur_(0), is_end_(true) { 
  index_.push_back(begin); 
  index_.push_back(end);
	end_index_ = index_;
  is_used_ = std::vector<bool>(end + 1, false);
}

  bool HasNext(); 

  int Next(); 

private:
  bool get_new_index();

  bool get_end_index();

  int begin_, end_;
  std::vector<int> index_;
  std::vector<int> end_index_;
  int cur_;
  bool is_end_;
  std::vector<bool> is_used_;
};
#endif
