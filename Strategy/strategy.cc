#include "strategy.h"

void Strategy::TrajMergeJoin(const list<int>& father_trajs, const std::list<int>& child_trajs, list<int>& join_res) const {
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

void BinaryStrategy::GetCandidateTrajs(const GridPanel* grid_panel, const SamplePoint& point, double dis, list<int>& candidates, bool is_end) {
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
			const list<int>& trajs = grid_panel->panel().GetTrajsInGrid(grid_index, is_end);
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
