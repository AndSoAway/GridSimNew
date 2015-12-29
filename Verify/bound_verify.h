#ifndef BOUND_VERIFY_H
#define BOUND_VERIFY_H
#include<unordered_map>
#include<vector>

#include "verify.h"

class BoundVerify : public BaseVerify {
public:
	virtual double verify(GridPanel& grid_panel, const Trajectory& target, const Trajectory& candidate, double threshold);

	virtual std::string name() { return "Bound Verify"; }	

private:
	int GetMaxGridSize(const std::vector<PointInfo>&);

	double getLowerAndUpper(GridPanel& gridpanel, const Trajectory& target, const Trajectory& candidate, int tar_id, std::vector<std::pair<bool, double>>& true_dis, std::unordered_map<int, std::vector<std::pair<double, int>>>& target_min_sort_grid, std::vector<double>& target_first_max, double *total_lower_dis, double *total_upper_dis, int grid_index);

	bool getDis(GridPanel& grid_panel, int test_id, std::unordered_map<int, std::vector<std::pair<double, int>>>& sort_min, std::vector<double>& first_max, const std::vector<PointInfo>& point_list);

	double GetMinDistance(const PointInfo& target, const Trajectory& candidates, const std::pair<int, int>& cur_seg);
};

#endif
