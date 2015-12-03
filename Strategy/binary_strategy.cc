#include "binary_strategy.h"
using namespace std;

#define TEST_DIS 500
#define THRESHOLD 10

#define CONVERT_TO_X(x) ((x) / LEN_PER_X)
#define CONVERT_TO_Y(y) ((y) / LEN_PER_Y)

void BinaryStrategy::FindCandidateTrajs(const GridPanel* grid_panel, const Trajectory& traj, double dis, list<int>& candidates) {
	const vector<SamplePoint>& point_list = traj.point_list();
	//int size = traj.point_list().size();	

	int cur = 0;
	if (HasNext()) {
		cur = Next();
		GetCandidateTrajs(grid_panel, point_list[cur], dis, candidates);	
	}

	if (HasNext()) {
		cur = Next();
		list<int> cur_can;
		GetCandidateTrajs(grid_panel, point_list[cur], dis, cur_can);
		list<int> res;
		TrajMergeJoin(candidates, cur_can, res);
		candidates.swap(res);
	}


	while (HasNext()) {
		cur = Next();
		list<int> cur_can, res;
		GetCandidateTrajs(grid_panel, point_list[cur], dis, cur_can);
		TrajMergeJoin(candidates, cur_can, res);
		candidates.swap(res);
	}
}

void BinaryStrategy::GetCandidateTrajs(const GridPanel* grid_panel, const SamplePoint& point, double dis, list<int>& candidates) {
  double x_dif = CONVERT_TO_X(dis);
	double y_dif = CONVERT_TO_Y(dis);
  double x_bottom = point.x() - x_dif ;
  double y_bottom = point.y() - y_dif;
  double x_upper = point.x() + x_dif;
  double y_upper = point.y() + y_dif; 
  pair<int, int> bottom = grid_panel->panel().GetGrid(x_bottom, y_bottom);
  pair<int, int> upper = grid_panel->panel().GetGrid(x_upper, y_upper);
	
	int point_sum = 0;
	for (int i = bottom.first; i <= upper.first; i++) {
		for (int j = bottom.second; j <= upper.second; j++) {
			pair<int, int> grid_index = make_pair(i, j);
			if (grid_panel->panel().IsContainPoint(grid_index)) {
				const set<int>& trajs = grid_panel->panel().GetTrajsInGrid(grid_index);
				set<int>::const_iterator cur_itor, pre_itor;
				cur_itor = trajs.cbegin();
				list<int>::iterator can_itor = candidates.begin();
				while (cur_itor != trajs.cend() && can_itor != candidates.end()) {
					while (can_itor != candidates.end()) {
						if (*cur_itor == *can_itor) {
							can_itor++;
							break;
						} else if (*cur_itor > *can_itor){
							can_itor++;
						} else {
							candidates.insert(can_itor, *cur_itor);
							break;
						}
					}
					if (can_itor == candidates.end()) {
						candidates.insert(can_itor, *cur_itor);
					}
					pre_itor = cur_itor;
					cur_itor++;
					while (*cur_itor == *pre_itor) {
						pre_itor = cur_itor;
						cur_itor++; 
					}
				} 
				point_sum += grid_panel->panel().GetPointsInGrid(grid_index).size();
			}
		}
	}
}

void BinaryStrategy::TrajMergeJoin(const list<int>& father_trajs, const std::list<int>& child_trajs, list<int>& join_res) const {
	list<int>::const_iterator fa_itor, ch_itor;
	fa_itor = father_trajs.cbegin();
	ch_itor = child_trajs.cbegin();
	while (fa_itor != father_trajs.cend() && ch_itor != child_trajs.cend()) {
		while (ch_itor != child_trajs.cend()) {
			if (*fa_itor == *ch_itor) {
				join_res.push_back(*fa_itor);
				ch_itor++;
				break;
			} else {
				ch_itor++;
			}
		}
		fa_itor++;
	}
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
	} else {
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
