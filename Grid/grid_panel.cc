#include <cstdio>
#include <vector>
#include <set>
#include "grid_panel.h"
#include "../Tra/trajectory_helper.h"
#include "../Tra/config.h"

using namespace std;

#define TEST_DIS 500
#define THRESHOLD 10


#define CONVERT_TO_X(x) ((x) / LEN_PER_X)
#define CONVERT_TO_Y(y) ((y) / LEN_PER_Y)

#define DEBUG 

void GridPanel::InsertTrajectory(const vector<Trajectory>& trajs) {
   for(auto traj : trajs) {
      InsertTrajectory(traj);
   }
}

void GridPanel::InsertTrajectory(const Trajectory& traj) {
//	printf("panel insert tra id %d\n", traj.id());
  if (trajs_.find(traj.id()) == trajs_.end()) {
    trajs_[traj.id()] = traj; 
    panel_.InsertTrajectory(traj);
  }
}

void GridPanel::GetCandidateTrajs(const SamplePoint& point, double dis, set<int>& candidates) const {
  double x_dif = CONVERT_TO_X(dis);
	double y_dif = CONVERT_TO_Y(dis);
  double x_bottom = point.x() - x_dif ;
  double y_bottom = point.y() - y_dif;
  double x_upper = point.x() + x_dif;
  double y_upper = point.y() + y_dif; 
  pair<int, int> bottom = panel_.GetGrid(x_bottom, y_bottom);
  pair<int, int> upper = panel_.GetGrid(x_upper, y_upper);

  //pair<int, int> self_grid = panel_.GetGrid(point);
//	printf("self: %d, %d\n", self_grid.first, self_grid.second);
 // printf("point x %lf, y %lf, dis %lf, bottom.x %d, bottom.y %d, upper.x %d, upper.y %d\n", point.x(), point.y(), dis, bottom.first, bottom.second, upper.first, upper.second);

	int point_sum = 0;
  for (int i = bottom.first < 0 ? 0 : bottom.first; i <= upper.first; i++) {
    for (int j = bottom.second; j <= upper.second; j++) {
		 // printf("i:%d, j:%d\n", i, j);
      pair<int, int> grid_index  = make_pair(i, j);

			/* get the candiadate trajs according to the points int grid*/
/*      if (panel_.IsContainPoint(grid_index)) {
        const vector<SamplePoint>& points = panel_.GetPointsInGrid(grid_index);
				point_sum += points.size();
        for (vector<SamplePoint>::const_iterator citor = points.cbegin(); citor != points.cend(); citor++) {
					printf("point's tra id:%d\n", citor->tra_id());
					if (candidates.find(citor->tra_id()) == candidates.end()) {
						candidates.insert(citor->tra_id());
					}
				} 
      }
			*/

			/* get candidate trajs stored in the grid */
			if (panel_.IsContainPoint(grid_index)) {
				const set<int>& trajs = panel_.GetTrajsInGrid(grid_index);
				candidates = trajs;
				point_sum += panel_.GetPointsInGrid(grid_index).size();
			}
    }
  }  
	
//	printf("Get candidate trajs: %ld, points sum : %d\n", candidates.size(), point_sum);
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

void GridPanel::BinaryFilter(const Trajectory& traj, set<int>& trajs) const {
  const vector<SamplePoint>& point_list = traj.point_list();
  int size = point_list.size();
//	printf("point size %d\n", size);
  BinaryStrategy binary_strategy(0, size - 1);

	if (binary_strategy.HasNext()) {
	  GetCandidateTrajs(point_list.at(binary_strategy.Next()), TEST_DIS, trajs);
//		printf("candidate trajs: %ld \n", trajs.size()); 
//		printf("----------------------------------------\n");
  }

	while( binary_strategy.HasNext() && trajs.size() >= THRESHOLD) {
	  int cur = binary_strategy.Next();
//		printf("cur: %d, ", cur);
		set<int> cur_trajs;
    GetCandidateTrajs(point_list.at(cur), TEST_DIS, cur_trajs);
//		printf("candidate trajs: %ld, ", cur_trajs.size());
    traj_filter(trajs, cur_trajs);
//		printf("filtered  trajs:%ld\n", trajs.size());
//		printf("----------------------------------------\n");
	}

  //printf("Points size:%ld\n", trajs.size());

/*
	for (vector<SamplePoint>::iterator itor = res.begin(); itor != res.end(); itor++) {
	  printf("trajectory id:%d\n", itor->tra()->id());
		if (trajs.find(itor->tra()) == trajs.end()) {
			trajs.insert(itor->tra());	
		}
	}
*/

	//printf("Trajs size:%ld\n", trajs.size());
}

void GridPanel::traj_filter(std::set<int>& father_trajs, const std::set<int>& child_trajs) const {	
	set<int> join_res;

/* O(mn) 算法 */
/*	for (set<int>:: const_iterator fa_itor = father_trajs.cbegin(); fa_itor != father_trajs.cend(); fa_itor++) {
	  for (set<int>::const_iterator ch_itor = child_trajs.cbegin(); ch_itor != child_trajs.cend(); ch_itor++) {
			if (*fa_itor == *ch_itor) {
				join_res.insert(*fa_itor);	
				break;
			}
		}
	}
*/
  set<int>::const_iterator begin_itor = child_trajs.cbegin();
  for (set<int>::const_iterator fa_itor = father_trajs.cbegin(); fa_itor != father_trajs.cend(); ++fa_itor) {
	  for(set<int>::const_iterator ch_itor = begin_itor; ch_itor != child_trajs.cend(); ++ch_itor) {
			if (*fa_itor == *ch_itor) {
				join_res.insert(*fa_itor);
				begin_itor = ch_itor;
				break;
			}
		}
	} 
	father_trajs = join_res;
}

bool BinaryStrategy::HasNext() { 
	int size = index_.size();
	if (is_end_) {
		return get_end_index();
	} else if (cur_ < size){
		return true;
	} else {
		  return get_new_index();
  }
}

int BinaryStrategy::Next() { 
	int val = index_[cur_++];
  is_used_[val] = true;
  return val;
}

bool BinaryStrategy::get_new_index() {
      std::vector<int> a_index;
      for (auto end : end_index_) {
      	for (auto old_num : index_) {
        	int a_num = (old_num + end) / 2;
          if (!is_used_[a_num]) {
          	a_index.push_back(a_num);
          }
        } 
			}
      if (a_index.empty()) {
			  cur_ = 0;
        return false;
			} else {
        index_ = a_index;
        cur_ = 0;
        return true;
      }
	}

bool BinaryStrategy::get_end_index() {
	int size = index_.size();
      if (cur_ < size) {
        return true;
      } else{
        is_end_ = false;
        int mid = (begin_ + end_) / 2;
        if (is_used_[mid]) {
          return false;
        } else {
          index_.clear();
          index_.push_back(mid);
          cur_ = 0;
          return true;
        }
      }
	}
