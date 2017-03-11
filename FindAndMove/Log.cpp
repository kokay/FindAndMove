#include <fstream>
#include "Log.hpp"

using namespace std;

LogItem::LogItem(const int moveFromRowNum, const string& moveFromText, const int moveToRowNum, const string& moveToText)
	:moveFromRowNum(moveFromRowNum), moveFromText(moveFromText), moveToRowNum(moveToRowNum), moveToText(moveToText){};

void Log::insert(const LogItem& logItem) {
	rows.push_back(logItem);
}

void Log::write(ofstream& logFile) {
	for (int i = 0; i < rows.size(); ++i) {
		logFile << rows[i].moveFromRowNum << "," << rows[i].moveFromText 
			<< "," << rows[i].moveToRowNum << "," << rows[i].moveToText << endl;
	}
}