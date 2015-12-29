#ifndef TRA_SAMPLE_POINT_H
#define TRA_SAMPLE_POINT_H
#include <time.h>
#include <string>
#include "point.h"
#include "utils.h"

class Trajectory;

class SamplePoint {
public:
	friend int fscanfPoint(FILE* stream, SamplePoint& point);

	friend bool operator==(const SamplePoint& lhs, const SamplePoint& rhs);

	friend bool operator!=(const SamplePoint& lhs, const SamplePoint& rhs);

	SamplePoint(time_t atime=time(NULL), int vehical_id=0, double x=0, double y=0, double angle=0, double speed=0, bool is_loaded=false)
	:vehical_id_(vehical_id), timestamp_(atime), heading_angle_to_north_(angle), speed_(speed), is_loaded_(is_loaded) {
		point_id_ = point_counter++;
		point_ = Point(x, y);
	}

	SamplePoint(char* line_str) {
		point_id_ = point_counter++;
		ExtractSamplePoint(line_str);
	}

	int point_id() const { return point_id_; }

	int vehical_id() const { return vehical_id_; }
	
	const time_t timestamp() const { return timestamp_; }

	const Point& point() const { return point_; }
	
	double x() const { return point_.x(); }

	double y() const { return point_.y(); }

	double heading_angle_to_north() const { return heading_angle_to_north_; }

	double speed() const { return speed_; }

	bool is_loaded() const { return is_loaded_; }  

	//void set_tra(Trajectory* p_tra) { p_tra_ = p_tra; }

	//const Trajectory* tra() const { return p_tra_; }

	void set_tra(const Trajectory *p_tra);

	int tra_id() const{
		return tra_id_;
	}

	void set_line(std::string line) { line_ = line; }

	const std::string& line() const{ return line_; }
	virtual ~SamplePoint() {}
protected:
	bool ExtractSamplePoint(char* line_str);
	
	long point_id_;
	int vehical_id_;
	time_t timestamp_;
	Point point_;
	double heading_angle_to_north_;
	double speed_;
	bool is_loaded_;
	//Trajectory* p_tra_;
	int tra_id_;
	
	static int point_counter;
	
	std::string line_;
};

struct PointInfo: public SamplePoint {
public:
//	PointInfo(): SamplePoint(), total_traj_count_(0), min_dis_(0), x_len(0), y_len(0) {}
	PointInfo(): SamplePoint(), min_dis_(0), x_len(0), y_len(0) {}

//	PointInfo(const SamplePoint& sample_point):SamplePoint(sample_point), total_traj_count_(0), min_dis_(0), x_len(0), y_len(0){
	PointInfo(const SamplePoint& sample_point):SamplePoint(sample_point), min_dis_(0), x_len(0), y_len(0){
	} 

	//PointInfo(const PointInfo& point_info) :SamplePoint(point_info){ }

	PointInfo& operator=(const SamplePoint& samplePoint);

//	int total_traj_count_;
	int traj_count_in_grid_;
	std::pair<int, int> grid_index_;
	double min_dis_;
	double x_len;
	double y_len;
//	int around_traj_size_;
	std::vector<std::pair<int, int>> neighbour_grid_;
};

inline bool min_dis_dsc(const PointInfo& left, const PointInfo& right) {
	return left.min_dis_ > right.min_dis_;
}

#endif
