#include "tra_test.h"
#include <cstdio>

using namespace std;

int main() {
  const char* file_path = "tra_data.txt";
  printf("Begin reading\n");
  trajectory_helper_test(file_path);  
  return 0;
}
