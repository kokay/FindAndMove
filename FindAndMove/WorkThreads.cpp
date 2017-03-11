#include <algorithm>
#include <fstream>
#include <iostream>
#include "WorkThreads.hpp"

using namespace std;

void WorkThreads::insert(thread t) {
	ts.push_back(move(t));
	progresses.push_back(0);
	logs.push_back(Log());
}

void WorkThreads::joinAll() {
	ts.push_back(thread(&WorkThreads::printProgress, this));
	for (thread& t : ts) {
		if (t.joinable()) t.join();
	}
}

void WorkThreads::progress(const int threadId, const float progress) {
	progresses[threadId] = progress;
}

void WorkThreads::log(const int threadId, const LogItem& logItem) {
	logs[threadId].insert(logItem);
}

void WorkThreads::printProgress() {
	while (any_of(progresses.begin(), progresses.end(), [](int i) { return i != 100; })) {
		for (int i = 0; i < progresses.size(); ++i)
			printf(" T%d %6.2f%% |", i, progresses[i]);
		printf("\n");
		this_thread::sleep_for(chrono::seconds(3));
	}

	for (int i = 0; i < progresses.size(); ++i)
		printf(" T%d %6.2f%% |", i, progresses[i]);
	printf("\n");
}

bool WorkThreads::generateLogFile(const string& path) {
	ofstream logFile(path, ofstream::out);
	if (!logFile.is_open()) {
		cout << "\nERROR" << endl;
		cout << "	PLACE  : " << path << endl;
		cout << "	REASON : '" << path << "' can not be opened." << endl;
		return false;
	}

	logFile << "MOVE_FROM Row Index,MOVE_FROM Text,MOVE_TO Row Index,MOVE_TO Text" << endl;
	for (int i = 0; i < logs.size(); ++i) {
		logs[i].write(logFile);
	}
	return true;
}