#include <vector>
#include <algorithm>
#include "panel.h"
#include "../Tra/utils.h"

using namespace std;

const vector<SamplePoint> Panel::empty_point_;

const list<int> Panel::empty_list_;

void Panel::InsertTrajectory(const Trajectory& traj) {
 	const vector<PointInfo>& point_list = traj.point_list();
	vector<PointInfo>::const_iterator itor = point_list.cbegin();
	InsertPoint(*itor, true);
	itor++;
 	while( itor != (point_list.cend() - 1)) {
	//	printf("point tra is :%d\n", point.tra()->id());
    		InsertPoint(*itor, false);
		itor++;
	}
	InsertPoint(*itor, true);

	int id = traj.id();
	int point_size = point_list.size();
	for (int index = 1; index < point_size - 1; index++) 
		InsertSegment(id, point_list[index - 1], point_list[index]);
}


void Panel::InsertPoint(const SamplePoint& point, bool end) {
   int x_grid_index = GetXIndex(point.x());
   int y_grid_index = GetYIndex(point.y());
    
   int x_lb = x_grid_index - 1;
   int x_up = x_grid_index + 1;
   int y_lb = y_grid_index - 1;
   int y_up = y_grid_index + 1;
  grid_set_[x_grid_index][y_grid_index].push_back(point);

	int tra_id = point.tra_id();
	
	//insert(point_traj_list_[x_grid_index][y_grid_index], tra_id);
	list<int>& point_id_list = point_traj_list_[x_grid_index][y_grid_index];
	if (point_id_list.empty() || tra_id != point_id_list.back()) {
		point_id_list.insert(point_id_list.end(), tra_id);
		for (int x = x_lb; x <= x_up; x++) {
			for (int y = y_lb; y <= y_up; y++) {
				int count = all_traj_count_[x][y];
				all_traj_count_[x][y] = count + 1;
			}
		}
	}
		
	if (end) {
		//insert(end_traj_list_[x_grid_index][y_grid_index], tra_id);
		list<int>& end_id_list = end_traj_list_[x_grid_index][y_grid_index];
		if (point_id_list.empty() || tra_id != point_id_list.back())
			end_id_list.insert(end_id_list.end(), tra_id);
	}
}

void Panel::InsertSegment(int tra_id, const SamplePoint& begin, const SamplePoint& end) {
	int x1_grid_index = GetXIndex(begin.x());
	int y1_grid_index = GetYIndex(begin.y());

	int x2_grid_index = GetXIndex(end.x());
	int y2_grid_index = GetYIndex(end.y());

	int min_x_grid_index = min(x1_grid_index, x2_grid_index);
	int max_x_grid_index = max(x1_grid_index, x2_grid_index);

	int min_y_grid_index = min(y1_grid_index, y2_grid_index);
	int max_y_grid_index = max(y1_grid_index, y2_grid_index);

	for (int x = min_x_grid_index; x <= max_x_grid_index; x++) {
		for (int y = min_y_grid_index; y <= max_y_grid_index; y++) {
			double x_left_log = double(x) * DISUNIT / LEN_PER_X + rectangle_.left_bottom().x();
			double y_left_lat = double(y) * DISUNIT / LEN_PER_Y + rectangle_.left_bottom().y();
			double x_right_log = double(x + 1) * DISUNIT / LEN_PER_X + rectangle_.left_bottom().x();
			double y_right_lat = double(y + 1) * DISUNIT / LEN_PER_Y + rectangle_.left_bottom().y();
			Point left_bottom(x_left_log, y_left_lat);
			Point right_upper(x_right_log, y_right_lat);
			Rectangle rect(left_bottom, right_upper);
			if (rect.IntersectSegment(begin.point(), end.point())) {
				list<int>& tra_id_list = point_traj_list_[x][y];
				//insert(tra_id_list, tra_id);
				if (tra_id_list.empty() || tra_id_list.front() != tra_id)
					tra_id_list.insert(tra_id_list.end(), tra_id);
			} 
		}
	}
}

int Panel::GetXIndex(double lon) const {
    double x_len = (lon - rectangle_.left_bottom().x()) * LEN_PER_X;    
    int x_index = int(x_len / DISUNIT); 
    return x_index;
}

int Panel::GetYIndex(double lat) const {
    double y_len = (lat - rectangle_.left_bottom().y()) * LEN_PER_Y;
    int y_index = y_len / DISUNIT;
    return y_index;
}

void Panel::GetPointInfo(PointInfo& point_info) const {
	const Point& point = point_info.point();
	pair<int, int> grid_index = GetGrid(point_info);
	point_info.grid_index_ = grid_index;
	double x_len = GetXLen(point.x());
	double y_len = GetYLen(point.y());
	point_info.x_len = x_len;
	point_info.y_len = y_len;
	double x_part = x_len - grid_index.first * DISUNIT;
	double y_part = y_len - grid_index.second * DISUNIT;
	double min_x = min(x_part, DISUNIT - x_part);
	double min_y = min(y_part, DISUNIT - y_part);
	point_info.min_dis_ = min(min_x, min_y);
}

string Panel::info() const {
   string str;
   str.append("Point distribution:\n");
   for(gridset::const_iterator itor = grid_set_.begin(); itor != grid_set_.end(); itor++) {
      int x_index = itor->first;
      for(grid::const_iterator griditor = itor->second.begin(); griditor != itor->second.end(); griditor++) {
         int y_index = griditor->first;
         int point_count = griditor->second.size();
         str.append(to_string(x_index)).append(", ").append(to_string(y_index)).append(" count: ").append(to_string(point_count)).append("\n");
      }
   }
   return str;
}

int Panel::GridSize() const {
  int total = 0;
  for (gridset::const_iterator itor = grid_set_.begin(); itor != grid_set_.end(); ++itor) {
    total += itor->second.size(); 
  }

  return total;
}

int Panel::PointSize() const {
  int total = 0;
  for(gridset::const_iterator itor = grid_set_.begin(); itor != grid_set_.end(); itor++) {
    for(grid::const_iterator griditor = itor->second.begin(); griditor != itor->second.end(); griditor++) {
      total += griditor->second.size();
    }
  }
  return total;
}

pair<int, int> Panel::GetGrid(const SamplePoint& sample_point) const {
  return GetGrid(sample_point.x(), sample_point.y());
}

pair<int, int> Panel::GetGrid(double x, double y) const {
  int x_index = GetXIndex(x);
  int y_index = GetYIndex(y);
  return make_pair(x_index, y_index); 
}

const vector<SamplePoint>& Panel::GetPointsInGrid(const std::pair<int, int>& grid_index) const {
	//printf("Enter GetPointsInGrid\n");
  if (!IsContainPoint(grid_index))
		return Panel::empty_point_; 
  return grid_set_.at(grid_index.first).at(grid_index.second);
} 


const std::list<int>& Panel::GetTrajsInGrid(const pair<int, int>& grid_index, bool is_end) const {
	if (is_end && IsContainEndPoint(grid_index)) {
		return end_traj_list_.at(grid_index.first).at(grid_index.second);
	} else if (IsContainPoint(grid_index)) {
		return point_traj_list_.at(grid_index.first).at(grid_index.second);
	}

	return Panel::empty_list_;
}

int Panel::GetAroundTrajCount(const std::pair<int, int>& grid_index) const {
	trajgridcount::const_iterator itor = all_traj_count_.find(grid_index.first);
	if (itor != all_traj_count_.end()) {
		unordered_map<int, int>::const_iterator a_itor = itor->second.find(grid_index.second);
		if (a_itor != itor->second.end()) {
			return a_itor->second;
		}
	}
	return 0;
}

int Panel::GetTrajCountInGrid(const pair<int, int>& grid_index, bool is_end) const {
	if (is_end) {
		trajgridlist::const_iterator itor = end_traj_list_.find(grid_index.first);
		if (itor != end_traj_list_.end()) {
			unordered_map<int, list<int>>::const_iterator a_itor = itor->second.find(grid_index.second);
			if (a_itor != itor->second.end())
				return a_itor->second.size();	
		}
	} else {
		trajgridlist::const_iterator itor = point_traj_list_.find(grid_index.first);
		if (itor != point_traj_list_.end()) {
			unordered_map<int, list<int>>::const_iterator a_itor = itor->second.find(grid_index.second);
			if (a_itor != itor->second.end())
				return a_itor->second.size();	
		}
	}
	return 0;
/*
	if (is_end && end_traj_list_.find(grid_index.first) != end_traj_list_.end() && end_traj_list_.at(grid_index.first).find(grid_index.second) != end_traj_list_.at(grid_index.first).end()) {
		return end_traj_list_.at(grid_index.first).at(grid_index.second).size();
	} else if (point_traj_list_.find(grid_index.first) != point_traj_list_.end() && point_traj_list_.at(grid_index.first).find(grid_index.second) != point_traj_list_.at(grid_index.first).end()) {
		return point_traj_list_.at(grid_index.first).at(grid_index.second).size();
	}
	return 0;
*/
}

const std::list<int>& Panel::GetEndTrajsInGrid(const pair<int, int>& grid_index) const {
	return end_traj_list_.at(grid_index.first).at(grid_index.second);
}

/*
const std::list<int>& Panel::GetTrajsInGrid(const pair<int, int>& grid_index) const {
  return traj_grid_list_.at(grid_index.first).at(grid_index.second);
}*/

bool Panel::IsContainPoint(const pair<int, int>& grid_index) const  {
  return (grid_set_.find(grid_index.first) != grid_set_.end() && grid_set_.at(grid_index.first).find(grid_index.second) != grid_set_.at(grid_index.first).end());
}

bool Panel::IsContainEndPoint(const pair<int, int>& grid_index) const {
  return (end_traj_list_.find(grid_index.first) != end_traj_list_.end() && end_traj_list_.at(grid_index.first).find(grid_index.second) != end_traj_list_.at(grid_index.first).end());
}

