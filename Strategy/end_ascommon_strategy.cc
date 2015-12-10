#include "end_ascommon_strategy.h"
#include <vector>

using namespace std;

void EndAsCommonStrategy::FindCandidateTrajs(const GridPanel* grid_panel, const Trajectory& traj, double dis, std::list<int>& candidates) {
    const vector<SamplePoint>& point_list = traj.point_list();
    int point_size = point_list.size();
    const SamplePoint& begin_point = point_list[0];
    const SamplePoint& end_point = point_list[point_size - 1];

    GetCandidateTrajs(grid_panel, begin_point, dis, candidates, false);
    
    list<int> cur_can;
    GetCandidateTrajs(grid_panel, end_point, dis, cur_can, false);
  
    list<int> res;
    TrajMergeJoin(candidates, cur_can, res);
    candidates.swap(res);

    FilterEnd(grid_panel, traj, candidates);
}
