#include "trajectory.h"
#include "config.h"
#include "utils.h"
#include "../Grid/grid_panel.h"
#include <cmath>

#define POINTMINSIZE 4
using namespace std;
void Trajectory::PushPoint(SamplePoint& point) {
	point.set_tra(this);
	//point_list_.push_back(point);
	PointInfo point_info(point);
	point_info.index_ = point_info_.size();
	point_info_.push_back(point_info);
//	UpdateSpecialPoint();
}

void Trajectory::CalPointInfo(const GridPanel* grid_panel, double dis) {
	for (vector<PointInfo>::iterator itor = point_info_.begin(); itor != point_info_.end(); itor++) {
		grid_panel->GetPointInfo(*itor, dis);			
	}
}

void Trajectory::CalGrid() {
	int point_size = point_info_.size();
	for (int index = 1; index < point_size - 1; ++index) {
		PointInfo& begin = point_info_[index -1];
		PointInfo& end = point_info_[index];
	
		double begin_x = begin.x_len / (double)DISUNT;
		double begin_y = begin.y_len / (double)DISUNIT;
	
		double end_x = end.x_len / (double)DISUNIT;
		double end_y = end.y_len / (double)DISUNIT;
		
		double delta_x = end_x - begin_x;
		double delta_y = end_y - end_y;
		
		pair<int, int> step(delta_x > 0 ? 1 : -1, delta_y > 0 ? 1 : -1);
		int x1_grid_index = begin.grid_index_.first;
		int y1_grid_index = begin.grid_index_.second;
		
		int x2_grid_index = end.grid_index_.first;
		int y2_grid_index = end.grid_inde_.second;

		if (x1_grid_index == x2_grid_index) {
			for (int y_grid_index = y1_grid_index; (y2_grid_index - y_grid_index) * step.second >= 0; y_grid_index += step.second) {
			InsertSegment(x1_grid_index, y_grid_index, index - 1, index);
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
					InsertSegment(x_grid_index, y_grid_index, index - 1, index);
					y_grid_index += step.second;
				}
				y_grid_index = next_y_grid_index;
				x_grid_index += step.first;
			}
			while ((y2_grid_index - y_grid_index) * step.second >= 0) {
				InsertSegment(x_grid_index, y_grid_index, index - 1, index);
				y_grid_index += step.second;
			}
		}
	} 	
}

void Trajectory::InsertSegment(int x_grid_index, int y_grid_index, int begin, int end) {
	trasegment& grid_segments = point_segement_map_[x_grid_index][y_grid_index];
	segmentset& tra_segments = grid_segments[tra_id];
	tra_segments.insert(make_pair(begin, end));	
}

void Trajectory::UpdateSpecialPoint() {
  int size = point_info_.size();
  if (size < POINTMINSIZE)
	return;
	
  const Point& first = point_info_[size - 4].point();
  const Point& second = point_info_[size - 3].point();
  const Point& third = point_info_[size - 2].point();
	
  double a = distSimplify(first.x(), first.y(), second.x(), second.y());

  double b = distSimplify(second.x(), second.y(), third.x(), third.y());
	
  double c = distSimplify(first.x(), first.y(), third.x(), third.y());

  double cos_val = (a * a + b * b - c* c) / (2 * a * b);

  double angle = acos(cos_val) * 180 / PI;

  if (angle <= ANGLETHRESHOLD) {
	special_point_.push_back(point_info_[size - 2]);	
  }
}

int Trajectory::tra_counter = 1;
