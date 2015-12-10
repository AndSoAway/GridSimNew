#ifndef LOGGING_H
#define LOGGING_H
#include <string>
#include <cstdio>
#include <cstdlib>
class Log {
public:
	static bool log(std::string str) { 
		char buf[128] = {0};
		time_t t;
		tm* local;
		t = time(NULL);
		local = localtime(&t);
		strftime(buf, 64, "%Y-%m-%d %H:%M:%S", local);
		FILE *log_file = fopen("log_file.txt", "a+");
		if(log_file == NULL) {
			printf("cannot open logfile: log_file.txt\n");
			return false;
		}
		fputs(buf, log_file);
		fputs("\n", log_file);
		fputs(str.c_str(), log_file);
		fputs("\n", log_file);
		fputs("--------------\n", log_file);
		fclose(log_file);
		return true;
	}
private:
	Log() { }
};
#endif
