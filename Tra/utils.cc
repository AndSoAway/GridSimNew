#include "utils.h"
#include <cmath>
#include <climits>
#include "trajectory.h"

#define TORADIAN(x) ((x) * PI / 180.0 )

using namespace std;

void split(const string& str, vector<string>& parts, const string& seperator) {
  string::size_type pos1, pos2;
  pos2 = str.find(seperator);
  pos1 = 0;
  while (string::npos != pos2) {
    parts.push_back(str.substr(pos1, pos2 - pos1));
    
    pos1 = pos2 + seperator.size();
    pos2 = str.find(seperator, pos1);
  }
 
  if (pos1 != str.size())
    parts.push_back(str.substr(pos1));
}

void strToTime(const string& date_str, time_t& time) {
  struct tm time_tm;  
  sscanf(date_str.c_str(), "%4d-%2d-%2d %2d:%2d:%2d", 
        &time_tm.tm_year,
        &time_tm.tm_mon,
        &time_tm.tm_mday,
        &time_tm.tm_hour,
        &time_tm.tm_min,
        &time_tm.tm_sec);
  time_tm.tm_year -= 1900;
  time_tm.tm_mon --;
  time_tm.tm_isdst = -1;
  time = mktime(&time_tm);
}

double distHaversineRAD(double lon1, double lat1, double lon2, double lat2) {
  double hsinX = sin(TORADIAN(lon1 - lon2)) * 0.5;
  double hsinY = sin(TORADIAN(lat1 - lat2)) * 0.5;
  double h = hsinY * hsinY + cos(TORADIAN(lat1)) * cos(TORADIAN(lat2)) * hsinX * hsinX;
  return 2 * atan2(sqrt(h), sqrt(1 - h)) * R;
}

double distSimplify(double lon1, double lat1, double lon2, double lat2) {
    double dx = lon1 - lon2;
    double dy = lat1 - lat2;
    double b = (lat1 + lat2) / 2;
    double lx = TORADIAN(dx) * R * cos(TORADIAN(b));
    double ly = R * TORADIAN(dy);
    return sqrt(lx * lx + ly * ly); 
}

int fscanfPoint(FILE* stream, SamplePoint& point) {
	struct tm time_tm;
	time_t tm;

	double y, x;
	int loaded_status;
  char line[100];
  fgets(line, 100, stream);
	point.set_line(line);
	int res = sscanf(line, "%d,%4d-%2d-%2d %2d:%2d:%2d,%lf,%lf,%lf,%lf,%d", 
		&point.vehical_id_, 
		&time_tm.tm_year,
		&time_tm.tm_mon,
		&time_tm.tm_mday,
		&time_tm.tm_hour,
		&time_tm.tm_min,
		&time_tm.tm_sec,
		&y,
		&x,
		&point.heading_angle_to_north_,
		&point.speed_,
		&loaded_status);
		
	if (res > 0) {
		time_tm.tm_year -= 1900;
		time_tm.tm_mon --;
		time_tm.tm_isdst = -1;
		tm = mktime(&time_tm);
    
		point.timestamp_ = tm;
		point.point_ = Point(x * ENLARGE, y * ENLARGE);
		point.is_loaded_ = bool(loaded_status);
	}
	return res;
}

bool operator==(const SamplePoint& lhs, const SamplePoint& rhs) {
	return lhs.point_id_ == rhs.point_id_;
}

bool operator!=(const SamplePoint& lhs, const SamplePoint& rhs) {
	return !(lhs == rhs);
}

double disSample(const SamplePoint& point1, const SamplePoint& point2) {
	const Point& begin = point1.point();
	const Point& end = point2.point();
	return disSample(begin, end);	
}

double disSample(const Point& begin, const Point& end) {
/*	double x_dif = (begin.x() - end.x()) * LEN_PER_X;
	double y_dif = (begin.y() - end.y()) * LEN_PER_Y;
	double dis = sqrt(x_dif * x_dif + y_dif * y_dif);
*/
	double dis = distSimplify(begin.x() / ENLARGE, begin.y() / ENLARGE, end.x() / ENLARGE, end.y() / ENLARGE);
	return dis;
}

void filterLoaded(FILE* stream, const string& file_name) {
	FILE* output = fopen(file_name.c_str(), "wb");

	double y, x;
	int loaded_status;

	char line[100];
	struct tm time_tm;
	int vehical_id_; 
	double heading_angle_to_north_;
	double speed_;

	while(fgets(line, 100, stream) != NULL) {
		sscanf(line, "%d,%4d-%2d-%2d %2d:%2d:%2d,%lf,%lf,%lf,%lf,%d", 
			&vehical_id_, 
			&time_tm.tm_year,
			&time_tm.tm_mon,
			&time_tm.tm_mday,
			&time_tm.tm_hour,
			&time_tm.tm_min,
			&time_tm.tm_sec,
			&y,
			&x,
			&heading_angle_to_north_,
			&speed_,
			&loaded_status);

		if (bool(loaded_status)) {
			fputs(line, output);
		}
	} 
	fclose(output);
}


bool order_by_count(pair<int, int> count1, pair<int, int> count2) {
	return count1.second < count2.second;
}

double minDistance(const SamplePoint& point, const Trajectory& tra) {
	double min_dis = double(ULONG_MAX); 
	const vector<PointInfo>& point_list = tra.point_list();
	int point_size = point_list.size();
	for (int index = 1; index < point_size; index++) {
		double cur_dis = minDistance(point, point_list.at(index), point_list.at(index - 1));
		if (cur_dis < min_dis) {
			min_dis = cur_dis;
		}
	} 
	return min_dis;
}

double minDistance(const SamplePoint& point, const SamplePoint& begin, const SamplePoint& end) {
	double a = disSample(point, begin);
	double b = disSample(point, end);	
	double c = disSample(begin, end);

	if (abs(a + b - c) < PRECISE) 
		return 0;

	if (c < PRECISE)
		return a;

	if (b * b >= a * a + c * c)
		return a;

	if (a * a >= b * b + c * c)
		return b;

	double cir = (a + b + c) / 2;
	double area = sqrt(cir * (cir - a) * (cir - b) * (cir - c));
  double res =  a * area / c;
	return res;
}
