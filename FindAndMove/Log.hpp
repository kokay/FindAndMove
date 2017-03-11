#ifndef LOG_HPP
#define LOG_HPP

#include <string>
#include <vector>

using namespace std;

struct LogItem {
	LogItem(const int moveFromRowNum, const string& moveFromText, const int moveToRowNum, const string& moveToText);
	
	int moveFromRowNum, moveToRowNum;
	string moveFromText, moveToText;
};

class Log {
public:
	void insert(const LogItem& logItem);
	void write(ofstream& logFile);
private:
	vector<LogItem> rows;
};

#endif // LOG_HPP