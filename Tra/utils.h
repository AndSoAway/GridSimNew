#ifndef TRA_UTILS_H
#define TRA_UTILS_H
#include <cstdio>
#include <vector>
#include <string>
#include <ctime>
#include <utility>
#include "config.h"
#include "sample_point.h"
#include "point.h"

class SamplePoint;

void split(const std::string&, std::vector<std::string>&, const std::string&);

void strToTime(const std::string& date_str, time_t& time);

double distHaversineRAD(double lon1, double lat1, double lon2, double lat2); 

double distSimplify(double lon1, double lat1, double lon2, double lat2);

int fscanfPoint(FILE* stream, SamplePoint& point);

bool operator==(const SamplePoint& lhs, const SamplePoint& rhs);

bool operator!=(const SamplePoint& lhs, const SamplePoint& rhs);

double disSample(const SamplePoint&, const SamplePoint&);

double disSample(const Point&, const Point&);

void filterLoaded(FILE* stream, const std::string& file_name);
bool order_by_count(std::pair<int, int> count1, std::pair<int, int> count2);

double minDistance(const SamplePoint& point, const Trajectory& tra);

double minDistance(const SamplePoint& point, const SamplePoint& begin, const SamplePoint& end);
#endif
