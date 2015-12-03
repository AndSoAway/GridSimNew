#include "../Tra/trajectory_helper.h"
#include <cstdio>
#include <vector>

using namespace std;

void trajectory_helper_test(const char* file_path) {
  FILE *tra_file = fopen(file_path, "rb");
  TrajectoryHelper *traj_helper = TrajectoryHelper::GetHelperInstance();
  vector<Trajectory> trajs;
  traj_helper->ExtractTrajectory(tra_file, trajs);
  printf("trajs size: %ld", trajs.size());
}
