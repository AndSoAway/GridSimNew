#include <cstdio>
#include <ctime>
#include <cmath>
#include "trajectory_helper.h"
#include "config.h"

using namespace std;

TrajectoryHelper TrajectoryHelper::trajectory_helper = TrajectoryHelper();

TrajectoryHelper* TrajectoryHelper::GetHelperInstance() {
    return &TrajectoryHelper::trajectory_helper;
}

void TrajectoryHelper::ExtractTrajectory(FILE* stream, vector<Trajectory>& tras) {
  Trajectory *p_tra = new Trajectory();
//	printf("tra's id: %d\n", p_tra->id());
  SamplePoint pre_point, cur_point;
  /*if (fgets(line, kLineLength, stream) != NULL) { 
    pre_point = SamplePoint(line);
    p_tra->PushPoint(pre_point);
  }*/
	if (fscanfPoint(stream, pre_point) > 0) {
		p_tra->PushPoint(pre_point);
	}
 
//  while(fgets(line, kLineLength, stream) != NULL) {
//    cur_point = SamplePoint(line);
	while (fscanfPoint(stream, cur_point) > 0) {
    if (abs(difftime(cur_point.timestamp(), pre_point.timestamp())) > TIMEINTERVAL) { //|| p_tra->point_size() >= MAXPOINTSIZE) { 
      if (p_tra->point_size() >= MINPOINTSIZE) {
        tras.push_back(*p_tra);
      }
      delete p_tra;
      p_tra = new Trajectory();
	//		printf("tra's id: %d\n", p_tra->id());
    }
    p_tra->PushPoint(cur_point);
    pre_point = cur_point;    
  }
  
  if (p_tra->point_size() >= MINPOINTSIZE) {
    tras.push_back(*p_tra);
  }
  delete p_tra;
} 
