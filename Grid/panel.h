#ifndef GRID_PANEL_H_
#define GRID_PANEL_H_ 1

#include <unordered_map>
#include <string>
#include <utility>
#include <vector>
#include <set>
#include <list>

#include "../Tra/trajectory.h"
#include "../Tra/sample_point.h"
#include "../Tra/config.h"
#include "rectangle.h"

struct TrajInfo{
	int tra_id;
	bool is_end;
};

class Panel{
public:
	Panel():width_(0), height_(0) { }
	Panel(const Rectangle& rect, double width=WIDTH, double height=HEIGHT) : rectangle_(rect), width_(width), height_(height){  }

	typedef std::unordered_map<int, std::unordered_map<int, std::vector<SamplePoint>>> gridset;
	typedef std::unordered_map<int, std::vector<SamplePoint>> grid;

	typedef std::unordered_map<int, std::unordered_map<int, std::set<int>>> trajgridset;
	typedef std::unordered_map<int, std::set<int>> trajgrid;
//	typedef std::unordered_map<int, std::unordered_map<int, std::set<int>>> trajInfoGridSet;
	typedef std::unordered_map<int, std::unordered_map<int, std::list<int>>> trajgridlist;
	typedef std::unordered_map<int, std::unordered_map<int, int>> trajgridcount;

	void InsertTrajectory(const Trajectory& traj);

	void InsertPoint(const SamplePoint& point, bool end=false);
	
	void InsertSegment(int tra_id, const SamplePoint& begin, const SamplePoint& end);

	void GetCandidatePoints(int tra_id, const SamplePoint& point, std::vector<SamplePoint>& candidates);

	std::string info() const;

	int GridSize() const;

	int PointSize() const;

	std::pair<int, int> GetGrid(const SamplePoint& sample_point) const;

	std::pair<int, int> GetGrid(double x, double y) const;

	void GetPointInfo(PointInfo& point_info, double dis) const;

	const std::vector<SamplePoint>& GetPointsInGrid(const std::pair<int, int>&) const;

	const std::list<int>& GetTrajsInGrid(const std::pair<int, int>&, bool is_end = false) const;

	int GetAroundTrajCount(const std::pair<int, int>& grid_index) const;

	int GetTrajCountInGrid(const std::pair<int, int>& grid_index, bool is_end = false) const ;

	const std::list<int>& GetEndTrajsInGrid(const std::pair<int, int>& grid_index) const;

	bool IsContainPoint(const std::pair<int, int>&) const;
	
	bool IsContainEndPoint(const std::pair<int, int>&) const;
private:
	int GetXIndex(double) const;
	int GetYIndex(double) const;
	
	inline double GetXLen(double) const;
	inline double GetYLen(double) const;

	Rectangle rectangle_;
	double width_;
	double height_;
	gridset grid_set_;
	trajgridlist point_traj_list_;
	trajgridlist end_traj_list_;
	trajgridcount all_traj_count_;
//	trajInfoGridSet trajinfo_grid_set_;
		//trajgridlist seg_traj_list_;
	static const std::vector<SamplePoint> empty_point_;
	static const std::list<int> empty_list_;
};

inline double Panel::GetXLen(double lon) const {
	double x_len = (lon - rectangle_.left_bottom().x()) * LEN_PER_X;
	return x_len;
}

inline double Panel::GetYLen(double lat) const {
	double y_len = (lat - rectangle_.left_bottom().y()) * LEN_PER_Y;
	return y_len;
}
#endif
