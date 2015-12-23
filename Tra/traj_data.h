#ifndef TRAJ_DATA_H_
#define TRAJ_DATA_H_
#include <utility>
#include <unordered_map>
#include "../Tra/trajectory.h"
//#include <vector>
class TrajData{
public:
	std::vector<Trajectory> trajs;
	std::vector<std::pair<int, int>> traj_index_point_count;
	std::unordered_map<int, int> traj_id_index;
	
	void push_back(Trajectory& traj) {
		trajs.push_back(traj);
		int index = trajs.size() - 1;
		traj_index_point_count.push_back(std::make_pair(index, traj.point_size()));
//		traj_id_index[traj.id()] = index;
	}
};
#endif
