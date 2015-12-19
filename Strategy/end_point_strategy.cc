#include "end_point_strategy.h"
#include <list>
using namespace std;

void EndPointStrategy::FindCandidateTrajs(const GridPanel* grid_panel, const Trajectory& traj, double dis, list<int>& candidates) {
    const vector<PointInfo>& point_list = traj.point_list();
    int point_size = point_list.size();
    const SamplePoint& begin_point = point_list[0];
    const SamplePoint& end_point = point_list[point_size - 1];

    GetCandidateTrajs(grid_panel, begin_point, dis, candidates, true);
    
    list<int> cur_can;
    GetCandidateTrajs(grid_panel, end_point, dis, cur_can, true);
  
    list<int> res;
    TrajMergeJoin(candidates, cur_can, res);
    candidates.swap(res);

    FilterEnd(grid_panel, traj, candidates);
}
