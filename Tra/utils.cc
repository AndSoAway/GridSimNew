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

int RegionHash(const pair<int, int>& cur_grid, const pair<int, int>& pre_grid) {
	int dif_x = cur_grid.first - pre_grid.first;
	int dif_y = cur_grid.second - pre_grid.second;
	return RegionHash(dif_x, dif_y);
}

int RegionHash(int dif_x, int dif_y) {
	int region_code = 1;
	if (dif_x > 0 && dif_y >= 0) {
		region_code = 0;
	} else if (dif_x <= 0 && dif_y > 0) {
		region_code = 1;
	} else if(dif_x < 0 && dif_y <= 0) {
		region_code = 2;
	} else if (dif_x >= 0 && dif_y < 0) {
		region_code = 3;
	}	
	return region_code;	
}

double getMinDis(const PointInfo& target_point vector<double, int>& min_dis_grid) {
	const pair<int, int>& point_grid = target_point.grid_index_;
	const vector<pair<int,int>>& around_grid = target_point.neighbour_gird_;
	int grid_size = around_grid.size();	
	double x_part = target_point.x_part;
	double y_part = target_point.y_part;
	for (int i = 0; i < grid_size; i++) {
		const pair<int, int>& grid_index = around_grid[i];
		if (grid_index.first == point_grid.first && grid_index.second == point_grid.second) {
			min_dis_grid.push_back(make_pair(0, i));
		} else if (grid_index.first == point_grid.first && grid_index.second == point_grid.second + 1) {
			min_dis_grid.push_back(make_pair(DISUNIT - y_part, i));
		} else if (grid_index.first == point_grid.first && grid_index.second == point_grid.second - 1) {
			min_dis_grid.push_back(make_pair(y_part, i));
		} else if (grid_index.first == point_grid.first - 1 && grid_index.second == point_grid.second) {
			min_dis_grid.push_back(make_pair(x_part, i));
		} else if (grid_index.first == point_grid.first - 1 && grid_index.second == point_grid.second - 1) {
			min_dis_grid.push_back(make_pair(sqrt(x_part * x_part + y_part * y_part), i));
		} else if (grid_index.first == point_grid.first - 1 && grid_index.second == point_grid.second + 1) {
			min_dis_grid.push_back(make_pair(sqrt(x_part * x_part + (DISUNIT - y_part) * (DISUNIT - y_part)), i));
		} else if (grid_index.first == point_grid.first + 1 && grid_index.second == point_grid.second) {
			min_dis_grid.push_back(make_pair(DISUNIT - x_part, i));
		} else if (grid_index.first == point_grid.first + 1 && grid_index.second == point_grid.second - 1) {
			min_dis_grid.push_back(make_pair(sqrt((DISUNIT - x_part ) * (DISUNIT - x_part) + y_part * y_part), i));
		} else if (grid_index.first == point_grid.first + 1 && grid_index.second == point_grid.second + 1) {
			min_dis_grid.push_back(make_pair(sqrt((DISUNIT - x_part) * ( DISUNIT - x_part) + (DISUNIT - y_part) * (DISUNIT - y_part)), i));
		}
	}	
}

double getMaxDis(const PointInfo& target_point, vector<double, int>& min_dis) {
	const pair<int, int>& grid_index = target_point.grid_index_;
	const vector<pair<int, int>>& around_grid = target_point.neighbour_grid_;
	int grid_size = around_grid.size();	
	double x_part = target_point.x_part;
	double y_part = target_point.y_part;
	double other_x = DISUNIT - x_part;
	double other_y = DISUNIT - y_part;
	double max_x_part = x_part > other_x ? x_part : other_x;
	double max_y_part = y_part > other_y ? y_part : other_y;
	for (int i = 0; i < grid_size; i++) {
		const pair<int, int>& grid_index = around_grid[i];
		double dis = 0;
		if (grid_index.first == point_grid.first && grid_index.second == point_grid.second) {
			dis = 0;
		} else if (grid_index.first == point_grid.first && grid_index.second == point_grid.second + 1) {
			dis = sqrt(max_x_part * max_x_part + (other_y + DISUNIT) * (DISUNIT + other_y));	
		} else if (grid_index.first == point_grid.first && grid_index.second == point_grid.second - 1) {
			dis = sqrt(max_x_part * max_x_part + (y_part + DISUNIT) * (y_part + DISUNIT));
		} else if (grid_index.first == point_grid.first - 1 && grid_index.second == point_grid.second) {
			dis = sqrt((x_part + DISUNIT) * (x_part + DISUNIT) + max_y_part * max_y_part);
		} else if (grid_index.first == point_grid.first - 1 && grid_index.second == point_grid.second - 1) {
			dis = sqrt((x_part + DISUNIT) * (x_part + DISUNIT) + (y_part + DISUNIT) * (y_part + DISUNIT));
		} else if (grid_index.first == point_grid.first - 1 && grid_index.second == point_grid.second + 1) {
			dis = sqrt((x_part + DISUNIT) * (x_part + DISUNIT) + (DISUNIT + other_y) * (DISUNIT + other_y))
		} else if (grid_index.first == point_grid.first + 1 && grid_index.second == point_grid.second) {
			dis = sqrt((DISUNIT + other_x) * (DISUNIT + other_x) + (max_y_part) * (max_y_part));
		} else if (grid_index.first == point_grid.first + 1 && grid_index.second == point_grid.second - 1) {
			dis = sqrt((DISUNIT + other_x) * (DISUNIT + other_x) + (y_part + DISUNIT) * (y_part + DISUNIT));
		} else if (grid_index.first == point_grid.first + 1 && grid_index.second == point_grid.second + 1) {
			dis = sqrt((other_x + DISUNIT ) * ( other_x + DISUNIT) + (other_y + DISUNIT) * (other_y + DISUNIT));
		}
		min_dis_grid.push_back(dis, i);
	}	
}
