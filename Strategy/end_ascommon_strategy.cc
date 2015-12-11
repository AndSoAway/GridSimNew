#include "end_ascommon_strategy.h"
#include <vector>
#include <string>
#include "../UnitTest/log.h"
using namespace std;

void EndAsCommonStrategy::FindCandidateTrajs(const GridPanel* grid_panel, const Trajectory& traj, double dis, std::list<int>& candidates) {
    const vector<SamplePoint>& point_list = traj.point_list();
    int point_size = point_list.size();
    const SamplePoint& begin_point = point_list[0];
    const SamplePoint& end_point = point_list[point_size - 1];

    clock_t get_can = clock();
    GetCandidateTrajs(grid_panel, begin_point, dis, candidates, false);
    
    list<int> cur_can;
    GetCandidateTrajs(grid_panel, end_point, dis, cur_can, false);
    get_can = clock() - get_can;
    string getCanInfo = "EndAsCommonStrategy two getCan time: " + to_string((double)get_can / CLOCKS_PER_SEC) + ", "; 

    list<int> res;
    clock_t merge_join = clock();
    TrajMergeJoin(candidates, cur_can, res);
    candidates.swap(res);
    merge_join = clock() - merge_join;
    getCanInfo = getCanInfo + " merge Join time: " + to_string((double)merge_join / CLOCKS_PER_SEC); 
    Log::log(1, getCanInfo);
}
