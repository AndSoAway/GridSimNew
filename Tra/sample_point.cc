#include <string>
#include <vector>
#include "../Tra/trajectory.h"
#include "sample_point.h"
#include "utils.h"
#include "config.h"


using namespace std;

int SamplePoint::point_counter = 1;

bool SamplePoint::ExtractSamplePoint(char *line_str) {
  string line(line_str);
  vector<string> parts;
  string seperator = ",";
  split(line, parts, seperator);

  vehical_id_ = stoi(parts[0]);
  strToTime(parts[1], timestamp_);
  double y = stod(parts[2]) * ENLARGE;
  double x = stod(parts[3]) * ENLARGE;
  point_ = Point(x, y);
  heading_angle_to_north_ = stod(parts[4]);
  speed_ = stod(parts[5]);
  is_loaded_ = bool(stoi(parts[6]));

  return true;
}

void SamplePoint::set_tra(const Trajectory *p_tra) {
	tra_id_ = p_tra->id();
}

PointInfo& PointInfo::operator=(const SamplePoint& rhs) {
	if (this != &rhs) {
		SamplePoint::operator=(rhs);
//		total_traj_count_ = 0;
		min_dis_ = 0;
		x_len = 0;
		y_len = 0;
	}
	return *this;
}
