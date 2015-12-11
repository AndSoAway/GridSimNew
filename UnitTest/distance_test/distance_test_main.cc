#include "distance_test.h"

void distance_test() {
  double disHav1 = distHaversineRAD(BJMAXX, BJMAXY, BJMINX, BJMAXY);
  double disHav2 = distHaversineRAD(BJMAXX, BJMAXY, BJMAXX, BJMINY);
  double disHav3 = distHaversineRAD(BJMAXX, BJMINY, BJMINX, BJMINY);
  double disHav4 = distHaversineRAD(BJMINX, BJMAXY, BJMINX, BJMINY);
 // double disSim = distSimplify(BJMAXX, BJMAXY, BJMINX, BJMINY);
  double disHav5 = distSimplify(BJMAXX, BJMAXY, BJMINX, BJMAXY);
  double disHav6 = distSimplify(BJMAXX, BJMAXY, BJMAXX, BJMINY);
  double disHav7 = distSimplify(BJMAXX, BJMINY, BJMINX, BJMINY);
  double disHav8 = distSimplify(BJMINX, BJMAXY, BJMINX, BJMINY);
  printf("disHav1: %lf, disHav5: %lf\n", disHav1, disHav5);
  printf("disHav2: %lf, disHav6: %lf\n", disHav2, disHav6);
  printf("disHav3: %lf, disHav7: %lf\n", disHav3, disHav7);
  printf("disHav4: %lf, disHav8: %lf\n", disHav4, disHav8);

 // printf("disSim: %lf\n", disSim);  
}

void calculate() {
  //double disHav1 = distHaversineRAD(116.304298, 39.923931, 116.350578, 39.939480);
  double disSim1 = distSimplify(116.304298, 39.923931, 116.350578, 39.939480);
	printf("dis %lf\n", disSim1);	
}

int main() {
  calculate();
  return 0;
}
