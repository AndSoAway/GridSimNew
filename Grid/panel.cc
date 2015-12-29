#include <vector>
#include <algorithm>
#include "panel.h"
#include "../Tra/utils.h"

using namespace std;

#define CONVERT_TO_X(x) ((x) / LEN_PER_X)
#define CONVERT_TO_Y(y) ((y) / LEN_PER_Y)

//const vector<SamplePoint> Panel::empty_point_;

const list<int> Panel::empty_list_;

void Panel::InsertTrajectory(const Trajectory& traj) {
 	const vector<PointInfo>& point_list = traj.point_list();
	int point_size = point_list.size();
/*	for (int i = 0; i < point_size; ++i) {
	  	InsertPoint(point_list[i], false);
	}
*/
	int id = traj.id();
	for (int index = 1; index < point_size - 1; index++) 
		InsertSegment(id, point_list[index - 1], point_list[index]);

}

/*
void Panel::InsertPoint(const PointInfo& point, bool end) {
	int x_grid_index = point.grid_index_.first;
	int y_grid_index = point.grid_index_.second;
	
	int x_lb = x_grid_index - 1;
	int x_up = x_grid_index + 1;
	int y_lb = y_grid_index - 1;
	int y_up = y_grid_index + 1;
	//grid_set_[x_grid_index][y_grid_index].push_back(point);

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
		
/ *
	if (end) {
		//insert(end_traj_list_[x_grid_index][y_grid_index], tra_id);
		list<int>& end_id_list = end_traj_list_[x_grid_index][y_grid_index];
		if (point_id_list.empty() || tra_id != point_id_list.back())
			end_id_list.insert(end_id_list.end(), tra_id);
	}
* /
}*/

void Panel::InsertSegment(int tra_id, const PointInfo& begin, const PointInfo& end) {
//line segment intersection in the grid algorithm
	double begin_x = begin.x_len / (double)DISUNIT;
	double begin_y = begin.y_len / (double)DISUNIT;		

	double end_x = end.x_len / (double)DISUNIT;
	double end_y = end.y_len / (double)DISUNIT;

	double delta_x = end_x - begin_x;
	double delta_y = end_y - begin_y;
	
	pair<int, int> step(delta_x > 0 ? 1 : -1, delta_y > 0 ? 1 : -1);
	

	int x1_grid_index = begin.grid_index_.first;
	int y1_grid_index = begin.grid_index_.second;

	int x2_grid_index = end.grid_index_.first;
	int y2_grid_index = end.grid_index_.second;
/*
	int min_x_grid_index = min(x1_grid_index, x2_grid_index);
	int max_x_grid_index = max(x1_grid_index, x2_grid_index);

	int min_y_grid_index = min(y1_grid_index, y2_grid_index);
	int max_y_grid_index = max(y1_grid_index, y2_grid_index);
*/
//	printf("begin (%lf, %lf), end (%lf, %lf)\n", begin.x(), begin.y(), end.x(), end.y());
//	printf("begin <%d %d>\n end <%d %d> %d\n", x1_grid_index, y1_grid_index, x2_grid_index, y2_grid_index, tra_id);
	if (x1_grid_index == x2_grid_index) {
		for (int y_grid_index = y1_grid_index; (y2_grid_index - y_grid_index) * step.second >= 0; y_grid_index += step.second) {
			InsertPoint(x1_grid_index, y_grid_index, tra_id, end);
		}
	} else {
		double slope = delta_y / delta_x;
		double intercept = end_y - slope * end_x;
		int x_grid_index = x1_grid_index;
		int y_grid_index = y1_grid_index;

		while (x_grid_index != x2_grid_index) {
			int next_grid_index = x_grid_index + step.first;
			double y_cor = slope * next_grid_index + intercept;
			int next_y_grid_index = (int)y_cor;
			while ((next_y_grid_index - y_grid_index) * step.second >= 0) {
				InsertPoint(x_grid_index, y_grid_index, tra_id, end);
				y_grid_index += step.second;
			}
			y_grid_index = next_y_grid_index;
			x_grid_index += step.first;
		}
		while ((y2_grid_index - y_grid_index) * step.second >= 0) {
			InsertPoint(x_grid_index, y_grid_index, tra_id, end);
			y_grid_index += step.second;
		}
	}
//	printf("min_x %d, min_y %d, max_x %d, max_y %d\n", min_x_grid_index, min_y_grid_index, max_x_grid_index, max_y_grid_index);
/*	for (int x = min_x_grid_index; x <= max_x_grid_index; x++) {
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
				if (tra_id_list.empty() || tra_id_list.back() != tra_id)
					tra_id_list.insert(tra_id_list.end(), tra_id);
			} 
		}
	}
*/
}

void Panel::InsertPoint(int x_grid_index, int y_grid_index, int tra_id, const PointInfo& end) {
	list<int>& tra_id_list = point_traj_list_[x_grid_index][y_grid_index];
//	printf("Insert <%d, %d> %d\n", x_grid_index, y_grid_index, tra_id);
	if (tra_id_list.empty() || tra_id_list.back() != tra_id) {
		tra_id_list.insert(tra_id_list.end(), tra_id);
		UpdateRelation(x_grid_index, y_grid_index, end);
	}
	
	for (int around_x = x_grid_index - 1; around_x <= x_grid_index + 1; ++around_x) {
		for (int around_y = y_grid_index -1; around_y <= y_grid_index + 1; ++around_y) {
			list<int>& around_id_list = around_traj_list_[around_x][around_y];
			if (around_id_list.empty() || around_id_list.back() != tra_id) {
				around_id_list.insert(around_id_list.end(), tra_id);
			}
		}
	}
}

void Panel::UpdateRelation(int x_grid_index, int y_grid_index, const PointInfo& end) {
	RelationInfo& relation = relation_grid_[x_grid_index][y_grid_index];
	const pair<int, int>& next_grid = end.grid_index_;
	int next_x_ = next_grid.first;
	int next_y_ = next_grid.second;

	int dif_x = next_x_ - x_grid_index;
	int dif_y = next_y_ - y_grid_index;
	if (dif_x == 0 && dif_y == 0)
		return;

	int region_code = RegionHash(dif_x, dif_y);
	relation.update(region_code);
	return;
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

void Panel::GetPointInfo(PointInfo& point_info, double dis) const {
	const Point& point = point_info.point();
	pair<int, int> grid_index = GetGrid(point_info);
	point_info.grid_index_ = grid_index;
	point_info.around_traj_size_ = GetAroundTrajCount(grid_index);
	double x_len = GetXLen(point.x());
	double y_len = GetYLen(point.y());
	point_info.x_len = x_len;
	point_info.y_len = y_len;
	
	double x_part = x_len - grid_index.first * DISUNIT;
	double y_part = y_len - grid_index.second * DISUNIT;
	double min_x = min(x_part, DISUNIT - x_part);
	double min_y = min(y_part, DISUNIT - y_part);
	point_info.min_dis_ = min(min_x, min_y);
	
	//printf("Grid<%d, %d>, x_len %lf, x_part %lf, y_part %lf, min_x %lf\n", grid_index.first, grid_index.second, x_len, x_part, y_part, min_x);
	double x_dif = CONVERT_TO_X(dis);
	double y_dif = CONVERT_TO_Y(dis);
	double x_bottom = point.x() - x_dif ;
	double y_bottom = point.y() - y_dif;
	double x_upper = point.x() + x_dif;
	double y_upper = point.y() + y_dif;
	pair<int, int> bottom = GetGrid(x_bottom, y_bottom);
	pair<int, int> upper = GetGrid(x_upper, y_upper); 

	for(int i = bottom.first; i <= upper.first; i++) {
		for(int j = bottom.second; j <= upper.second; j++) {
			point_info.neighbour_grid_.push_back(make_pair(i, j));
		}
	}
}

/*
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
*/

/*
int Panel::GridSize() const {
	int total = 0;
	for (gridset::const_iterator itor = grid_set_.begin(); itor != grid_set_.end(); ++itor) {
	total += itor->second.size(); 
	}

	return total;
}
*/
/*
int Panel::PointSize() const {
	int total = 0;
	for(gridset::const_iterator itor = grid_set_.begin(); itor != grid_set_.end(); itor++) {
		for(grid::const_iterator griditor = itor->second.begin(); griditor != itor->second.end(); griditor++) {
	  		total += griditor->second.size();
		}
	}
	return total;
}
*/

pair<int, int> Panel::GetGrid(const SamplePoint& sample_point) const {
	return GetGrid(sample_point.x(), sample_point.y());
}

pair<int, int> Panel::GetGrid(double x, double y) const {
	int x_index = GetXIndex(x);
	int y_index = GetYIndex(y);
	return make_pair(x_index, y_index); 
}

/*
const vector<SamplePoint>& Panel::GetPointsInGrid(const std::pair<int, int>& grid_index) const {
	//printf("Enter GetPointsInGrid\n");
	if (!IsContainPoint(grid_index))
		return Panel::empty_point_; 
	return grid_set_.at(grid_index.first).at(grid_index.second);
} 
*/

/*
const std::list<int>& Panel::GetTrajsInGrid(const pair<int, int>& grid_index, bool is_end) const {
	if (is_end && IsContainEndPoint(grid_index)) {
		return end_traj_list_.at(grid_index.first).at(grid_index.second);
	} else if (IsContainPoint(grid_index)) {
		return point_traj_list_.at(grid_index.first).at(grid_index.second);
	}

	return Panel::empty_list_;
}
*/
int Panel::GetAroundTrajCount(const std::pair<int, int>& grid_index) const {
	trajgridlist::const_iterator itor = around_traj_list_.find(grid_index.first);
	if (itor != around_traj_list_.end()) {
		gridlist::const_iterator a_itor = itor->second.find(grid_index.second);
		if (a_itor != itor->second.end()) {
			return a_itor->second.size();
		}
	}
	return 0;
}

const std::list<int>& Panel::GetTrajsAroundGrid(const std::pair<int, int>& grid_index) const {
	trajgridlist::const_iterator itor = around_traj_list_.find(grid_index.first);
	if (itor != around_traj_list_.end()) {
		gridlist::const_iterator a_itor = itor->second.find(grid_index.second);
		if (a_itor != itor->second.end()) {
			return a_itor->second;
		}
	}	
	return Panel::empty_list_;
}

const RelationInfo* Panel::GetRelationInfo(const std::pair<int, int>& grid_index) const {
	relationgridlist::const_iterator itor = relation_grid_.find(grid_index.first);
	if (itor != relation_grid_.end()) {
		unordered_map<int, RelationInfo>::const_iterator a_itor = itor->second.find(grid_index.second);
		if (a_itor != itor->second.end())
			return &(a_itor->second);
	}
	return NULL;
}

int Panel::GetTrajCountInGrid(const pair<int, int>& grid_index, bool is_end) const {
	const list<int>& traj_list = GetTrajsInGrid(grid_index, is_end);
	return traj_list.size();
}

const std::list<int>& Panel::GetTrajsInGrid(const pair<int, int>& grid_index, bool is_end) const {
/*	if (is_end) {
		trajgridlist::const_iterator itor = end_traj_list_.find(grid_index.first);
		if (itor != end_traj_list_.end()) {
			unordered_map<int, list<int>>::const_iterator a_itor = itor->second.find(grid_index.second);
			if (a_itor != itor->second.end())
				return a_itor->second;	
		}
	} else {*/
		trajgridlist::const_iterator itor = point_traj_list_.find(grid_index.first);
		if (itor != point_traj_list_.end()) {
			unordered_map<int, list<int>>::const_iterator a_itor = itor->second.find(grid_index.second);
			if (a_itor != itor->second.end())
				return a_itor->second;	
		}
//	}
	return Panel::empty_list_;
/*
	if (is_end && end_traj_list_.find(grid_index.first) != end_traj_list_.end() && end_traj_list_.at(grid_index.first).find(grid_index.second) != end_traj_list_.at(grid_index.first).end()) {
		return end_traj_list_.at(grid_index.first).at(grid_index.second).size();
	} else if (point_traj_list_.find(grid_index.first) != point_traj_list_.end() && point_traj_list_.at(grid_index.first).find(grid_index.second) != point_traj_list_.at(grid_index.first).end()) {
		return point_traj_list_.at(grid_index.first).at(grid_index.second).size();
	}
	return 0;
*/
}
/*
const std::list<int>& Panel::GetEndTrajsInGrid(const pair<int, int>& grid_index) const {
	return end_traj_list_.at(grid_index.first).at(grid_index.second);
}
*/
/*
const std::list<int>& Panel::GetTrajsInGrid(const pair<int, int>& grid_index) const {
  return traj_grid_list_.at(grid_index.first).at(grid_index.second);
}*/

/*
bool Panel::IsContainPoint(const pair<int, int>& grid_index) const  {
  return (grid_set_.find(grid_index.first) != grid_set_.end() && grid_set_.at(grid_index.first).find(grid_index.second) != grid_set_.at(grid_index.first).end());
}
*/

/*
bool Panel::IsContainEndPoint(const pair<int, int>& grid_index) const {
  return (end_traj_list_.find(grid_index.first) != end_traj_list_.end() && end_traj_list_.at(grid_index.first).find(grid_index.second) != end_traj_list_.at(grid_index.first).end());
}
*/
