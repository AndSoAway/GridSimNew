#ifndef TRA_TRAJECTORY_HELPER_H
#define TRA_TRAJECTORY_HELPER_H 1
#include <vector>
#include <cstdio>
#include "trajectory.h"
#include "sample_point.h"
#include "traj_data.h"
#define  kLineLength 100

class TrajectoryHelper {
public:
  static TrajectoryHelper* GetHelperInstance();

  void ExtractTrajectory(FILE* stream, TrajData& traj_data);

private:
  TrajectoryHelper() { }
  
  char line[kLineLength];
  static TrajectoryHelper trajectory_helper;
};
#endif
