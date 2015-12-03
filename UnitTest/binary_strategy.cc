#include <cstdio>
#include "../Grid/grid_panel.h"

using namespace std;

int main() {
  BinaryStrategy binary_strategy(0, 20);
  while (binary_strategy.HasNext()) {
    printf("%d\n", binary_strategy.Next());
  } 
  return 0;
}
