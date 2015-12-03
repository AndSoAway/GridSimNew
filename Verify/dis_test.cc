#include "verify.h"
#include "../Tra/point.h"
#include "../Tra/utils.h"

int main() {
	Point begin(116.302330, 39.952461);
	Point end(116.303139,39.951721);
	double dis = disSample(begin, end);	
	printf("dis is %lf\n", dis);
}
