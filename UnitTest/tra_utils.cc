#include "tra_utils.h"
#include "../Tra/sample_point.h"
#include "../Tra/utils.h"
#include <string>
#include <cstdio>

#define DIST 20
#define FILE_NUM 10
extern const char * file_paths[FILE_NUM];

/*  = {
   "tra_data/tra_data0.txt",
   "tra_data/tra_data1.txt",
   "tra_data/tra_data2.txt",
   "tra_data/tra_data3.txt",
   "tra_data/tra_data4.txt",
   "tra_data/tra_data5.txt",
   "tra_data/tra_data6.txt",
   "tra_data/tra_data7.txt",
   "tra_data/tra_data8.txt",
   "tra_data/tra_data9.txt",
 };
*/

using namespace std;
void simplify() { 
  for (int i = 0; i < FILE_NUM; i++) {
			string file_ori = file_paths[i];
			file_ori.insert(0, "sim");
      FILE *file = fopen(file_paths[i], "rb");
			FILE *out_file = fopen(file_ori.c_str(), "wb");
			SamplePoint pre, aft;
      fscanfPoint(file, pre);
			int sum = 0;
			int sim = 0;
			//fputs(pre.line().c_str(), out_file);
			while (fscanfPoint(file, aft) > 0) {
				double pre_x = pre.point().x() / ENLARGE;
				double pre_y = pre.point().y() / ENLARGE;
				double aft_x = aft.point().x() / ENLARGE;
 				double aft_y = aft.point().y() / ENLARGE;
				double dist = distHaversineRAD(pre_x, pre_y, aft_x, aft_y);
				if (dist > 20) {
					//fputs(aft.line().c_str(), out_file);
					//printf("dis %lf\n", dist);
					aft = pre;
				} else {
					//printf("dis %lf\n", dist);
					sim++;
				}
				sum++;
			}
			printf("total %d, sim %d\n", sum , sim);
			fclose(file);
			fclose(out_file);
  }
}
