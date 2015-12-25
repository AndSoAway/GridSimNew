#include "correlation_strategy.h"

void CorrelationStrategy::FindCandidateTrajs(const GridPanel* grid_panel, Trajectory& traj, double dis, std::list<int>& candidates) {
	vector<PointInfo>& point_list = traj.point_list();
	
	int point_size = point_list.size();
	for (int i = 0; i < point_size; ++i) {
		PointInfo& cur_point = point_list[i];
		pair<int, int>& cur_grid = cur_point.grid_index_;
		const RelationInfo* relation = grid_panel->panel().GetRelationInfo(cur_grid);
		if (relation == NULL)
				
	}		
}
