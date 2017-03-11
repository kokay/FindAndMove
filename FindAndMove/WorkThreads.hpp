#ifndef PRINT_PROGRESS_THREADS_HPP
#define PRINT_PROGRESS_THREADS_HPP

#include <thread>
#include <vector>
#include "Log.hpp"

using namespace std;

class WorkThreads {
public:
	void insert(thread t);
	void progress(const int threadId, const float progress);
	void log(const int threadId, const LogItem& logItem);
	void joinAll();
	bool generateLogFile(const string& path);
private:
	void printProgress();
	vector<thread> ts;
	vector<float> progresses;
	vector<Log> logs;
};

#endif // PRINT_PROGRESS_THREADS_HPP
