#ifndef TRA_TRAJECTORY_H
#define TRA_TRAJECTORY_H
#include <vector>
#include <cstdio>
#include "sample_point.h"
#include "utils.h"
//#include "../Grid/grid_panel.h"

//class SamplePoint;
class GridPanel;

class Trajectory {
public:
	Trajectory() { tra_id_ = tra_counter++; }
	 
	void PushPoint(SamplePoint& point);

	const std::vector<PointInfo>& point_list() const { return point_info_; }
	const std::vector<SamplePoint>& special_point() const { return special_point_; }
	const std::vector<PointInfo>& pointinfo_list() const { return point_info_; }
	int id() const { return tra_id_; }
	void set_id(int id) { tra_id_ = id; }    
	//int point_size() const { return point_list_.size(); } 
	int point_size() const { return point_info_.size(); }
	void CalPointInfo(const GridPanel* grid_panel, double dis);

	void CalGrid();

	void InsertSegment(int x_grid_index, int y_grid_index, const PointInfo& begin, const PointInfo&);

	typedef std::unordered_set<std::pair<int, int>, pairhash> segmentset;
	typedef std::unordered_map<int, segmentset> trasegment;
	typedef unordered_map<int, std::unordered_map<int, trasegment>> gridsegments;

private:
	void UpdateSpecialPoint();
	int tra_id_;
	//std::vector<SamplePoint> point_list_;	
	std::vector<SamplePoint> special_point_;
	std::vector<PointInfo> point_info_;
	static int tra_counter;
	
	gridsegments point_segment_map_; 
};
#endif
