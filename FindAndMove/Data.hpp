#ifndef DATA_HPP
#define DATA_HPP

#include <vector>
#include "WorkThreads.hpp"

using namespace std;

typedef vector<string> Row;
typedef vector<string> ComparedText;

class Data {
public:
	static bool generateLogFile(const string& path);

	bool prepare(const string& dataPath, const string& comparedHeader);
	void addHeaderExtra(const Row& otherHeader, const int startCol);
	const Row& getHeader() { return table[HEADER]; };
	void findSimilerRowForAll(Data& comparedData, const int accurary,
		const int startExtraIdx, const int numCores);
	bool generateResultDataFile(const string& path);

private:
	static const int HEADER = 1;
	static const int START_ROW = 2;
	static WorkThreads threads;

	bool readFile(const string& path);
	bool generateComparedTexts(const string& comparedHeader);
	string clean(const string& word);
	void addRowExtra(const int addedRow, const Row& otherRow, const int startCol);
	void addRowExtraEmpty(const int addedRow, const int numEmptyCells);
	void findSimilerRowForPart(Data& comparedData, const int startRow, const int endRow,
		const int accurary, const int startExtraIdx, const int threadId);
	static bool rowsAreSimiler(vector<vector<int>>& memo,
		const ComparedText& t1, const ComparedText& t2, const int accuracy);

	vector<Row> table;
	vector<ComparedText> comparedTexts;
	int comparedIdx = 0;
	int maxSizeOfComparedText = 0;
};

#endif // DATA_HPP
