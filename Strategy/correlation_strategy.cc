#include <vector>
#include "../Tra/utils.h"
#include "correlation_strategy.h"

using namespace std;
void CorrelationStrategy::FindCandidateTrajs(const GridPanel* grid_panel, Trajectory& traj, double dis, std::list<int>& candidates) {
	vector<PointInfo>& point_list = traj.pointinfo_list();	
	int point_size = point_list.size();
	
	pair<int, int> *pre_grid, *cur_grid;
//	pair<int, int> *min_grid;
	int min_size_point;
	const RelationInfo *pre_relation, *cur_relation;
	//const RelationInfo *min_relation;
	int min_tra_count; 
	
	PointInfo& first_point = point_list[0];
	pre_grid = &first_point.grid_index_;
	pre_relation = grid_panel->panel().GetRelationInfo(*pre_grid);
	
//	min_grid = pre_grid;
	min_size_point = 0;
	//min_relation = pre_relation;
	//min_tra_count = grid_panel->panel().GetAroundTrajCount(*pre_grid);
	min_tra_count = first_point.traj_count_in_grid_; 
	bool is_first = true;
	for (int i = 1; i < point_size; ++i) {
		PointInfo& cur_point = point_list[i];
		cur_grid = &cur_point.grid_index_;
		cur_relation = grid_panel->panel().GetRelationInfo(*cur_grid);
		
		//
		bool same_region = true;
		if (pre_relation != NULL) {
			same_region = GetSameRegion(*cur_grid, *pre_grid, pre_relation);
		}
		
		if (same_region) {
//			int cur_tra_count = grid_panel->panel().GetAroundTrajCount(*cur_grid);
			int cur_tra_count = cur_point.traj_count_in_grid_;
			if (cur_tra_count < min_tra_count) {
//			min_relation = cur_relation;
//				min_grid = cur_grid;
				min_size_point = i;
			}
		} else {
//			const list<int>& min_tra_list = grid_panel->panel().GetTrajsAroundGrid(*min_grid);
			list<int> min_tra_list;
			GetCandidateTrajs(grid_panel, point_list[min_size_point], dis, min_tra_list, false);
			if (is_first) {
				candidates = min_tra_list;
			}else {
				list<int> res;
				TrajMergeJoin(candidates, min_tra_list, res);
				candidates = res;	
			}
//			min_relation = cur_relation;
//			min_grid = cur_grid;
			min_size_point = i;
		}	
		pre_grid = cur_grid;
		pre_relation = cur_relation;
	}		
//	const list<int>& min_tra_list = grid_panel->panel().GetTrajsAroundGrid(*min_grid);
	list<int> min_tra_list;
	GetCandidateTrajs(grid_panel, point_list[min_size_point], dis, min_tra_list, false);
	if (is_first) {
		candidates = min_tra_list;
	} else {
		list<int> res;
		TrajMergeJoin(candidates, min_tra_list, res);
		candidates = res;	
	}

	//printf("id %d, can size %d\n", traj.id(), candidates.size());
}

bool CorrelationStrategy::GetSameRegion(const pair<int, int>& cur_grid, const pair<int, int>& pre_grid, const RelationInfo* pre_relation_info) {
	int region_code = RegionHash(cur_grid, pre_grid);
	double percent = pre_relation_info->get_percent(region_code);
	return (percent >= RELATIONTHRESHOLD);	
}
