#include "correlation_strategy.h"

void CorrelationStrategy::FindCandidateTrajs(const GridPanel* grid_panel, Trajectory& traj, double dis, std::list<int>& candidates) {
	vector<PointInfo>& point_list = traj.point_list();
	
	pair<int, int>& grid_index = point_list[0].grid_index_;
	list<int>& around_list = grid_panel->panel().GetTrajsAroundGrid(grid_index);
	int cur_size = around_list.size();
	
	for (int i = 1; i < cur_size; i++) {
		PointInfo& cur_point = point_list[i];
		for(int index = 0; index < cur_size; index++) {
			pair<int, int>& 
		}		
	}
}
