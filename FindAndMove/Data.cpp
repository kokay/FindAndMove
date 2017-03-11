#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "Data.hpp"

using namespace std;

bool Data::prepare(const string& dataPath, const string& comparedHeader) {
	if (!readFile(dataPath)) return false;
	if (!generateComparedTexts(comparedHeader)) return false;
	return true;
}

bool Data::readFile(const string& path) {
	ifstream ifs(path, ifstream::in);
	if (!ifs.is_open()) {
		cout << "\nERROR" << endl;
		cout << "	PLACE  : " << path << endl;
		cout << "	REASON : '" << path << "' can not be opened." << endl;
		return false;
	}

	table.push_back({}); // make row start from 1
	int lastRowIdx = 0;

	string line, word;
	while (getline(ifs, line)) {
		table.push_back({});
		lastRowIdx++;

		istringstream ss(line);
		while (getline(ss, word, ',')) {
			table.back().push_back(word);
		}
		if (lastRowIdx >= 2 &&
			table[lastRowIdx].size() != table[lastRowIdx - 1].size()) {
			cout << "\nERROR" << endl;
			cout << "	PLACE  : " << path << endl;
			cout << "	REASON : '" << path << "' has different number of colums on "
				<< lastRowIdx - 1 << " and  " << lastRowIdx << endl;
			return false;
		}
	}
	return true;
}

bool Data::generateComparedTexts(const string& comparedHeader) {
	static string HTTP = "http:";
	vector<string>::iterator it = find(table[HEADER].begin(), table[HEADER].end(), comparedHeader);
	if (it == table[HEADER].end()) {
		cout << "\nERROR" << endl;
		cout << "	PLACE  : " << comparedHeader << endl;
		cout << "	REASON : '" << comparedHeader << "' is not on the table." << endl;
		return false;
	}

	comparedTexts.resize(table.size());
	string word;
	comparedIdx = (int)(it - table[HEADER].begin());
	for (int i = START_ROW; i < table.size(); ++i) {
		istringstream ss(table[i][comparedIdx]);
		while (ss >> word) {
			if (word.substr(0, HTTP.size()) == HTTP) continue;
			word = clean(word);
			if (!word.empty())
				comparedTexts[i].push_back(word);

			if (comparedTexts[i].size() > maxSizeOfComparedText)
				maxSizeOfComparedText = comparedTexts[i].size();
		}
	}
	return true;
}

string Data::clean(const string& word) {
	string cleaned;
	for (int i = 0; i < word.size(); ++i) {
		if (word[i] != '.' && word[i] != '?')
			cleaned += tolower(word[i]);
	}
	return cleaned;
}

void Data::addHeaderExtra(const Row& otherHeader, const int startCol) {
	for (int i = startCol; i < otherHeader.size(); ++i) {
		table[HEADER].push_back(otherHeader[i]);
	}
}

void Data::addRowExtra(const int addedRowIdx, const Row& otherRow, const int startCol) {
	for (int i = startCol; i < otherRow.size(); ++i) {
		table[addedRowIdx].push_back(otherRow[i]);
	}
}

void Data::addRowExtraEmpty(const int addedRow, const int numEmptyCells) {
	for (int i = 0; i < numEmptyCells; ++i) {
		table[addedRow].push_back("");
	}
}

void Data::findSimilerRowForAll(Data& moveFrom, const int accurary, const int startExtraIdx, const int numCores) {
	for (int i = 0; i < numCores; ++i) {
		int startRow = (i * ((int)table.size() - Data::START_ROW) / numCores) + Data::START_ROW;
		int endRow = ((i + 1) * ((int)table.size() - Data::START_ROW) / numCores) + Data::START_ROW - 1;
		threads.insert(thread(&Data::findSimilerRowForPart, this, moveFrom, startRow, endRow, accurary, startExtraIdx, i));
	}
	threads.joinAll();
}

void Data::findSimilerRowForPart(Data& moveFrom, const int startRow, const int endRow,
	const int accuracy, const int startExtraIdx, const int threadId) {
	vector<vector<int>> memo(maxSizeOfComparedText,
		vector<int>(moveFrom.maxSizeOfComparedText));
	for (int row = startRow; row <= endRow; ++row) {
		bool found = false;
		for (int moveFromDataRow = Data::START_ROW; moveFromDataRow < moveFrom.table.size(); ++moveFromDataRow) {
			if (Data::rowsAreSimiler(memo, comparedTexts[row], moveFrom.comparedTexts[moveFromDataRow], accuracy)) {
				threads.log(threadId, LogItem(moveFromDataRow, moveFrom.table[moveFromDataRow][moveFrom.comparedIdx], row, table[row][comparedIdx]));
				addRowExtra(row, moveFrom.table[moveFromDataRow], startExtraIdx);
				found = true;
				break;
			}
		}
		if (!found) addRowExtraEmpty(row, (int)moveFrom.table[Data::HEADER].size() - startExtraIdx);
		threads.progress(threadId, ((row - startRow + 1) / (float)(endRow - startRow + 1)) * 100);
	}
}

bool Data::rowsAreSimiler(vector<vector<int>>& memo, const ComparedText& t1, const ComparedText& t2, const int accuracy) {
	if (t1.size() == 0 || t2.size() == 0) return false;
	if (t1.size() == 1 && t2.size() == 1) return t1 == t2;

	int bigger = (int)max(t1.size(), t2.size());
	int smaller = (int)min(t1.size(), t2.size());
	if ((smaller / (float)bigger) < (accuracy / 100.0)) return false;

	int worstStep = (int)(bigger * (100 - accuracy) / 100.0);
	for (int i = 0; i < t1.size(); ++i) memo[i][0] = i;
	for (int i = 0; i < t2.size(); ++i) memo[0][i] = i;

	int match, insert, remove;
	for (int i = 1; i < t1.size(); ++i) {
		int lowest = max(memo.size(), memo[i].size());
		for (int j = 1; j < t2.size(); ++j) {
			if (t1[i] == t2[j]) match = memo[i - 1][j - 1];
			else match = memo[i - 1][j - 1] + 1;
			insert = memo[i][j - 1] + 1;
			remove = memo[i - 1][j] + 1;
			memo[i][j] = min(match, min(insert, remove));
			lowest = min(lowest, memo[i][j]);
		}
		if (lowest > worstStep) return false;
	}
	return true;
}

bool Data::generateResultDataFile(const string& path) {
	ofstream resultDataFile(path);
	if (!resultDataFile.is_open()) {
		cout << "\nERROR" << endl;
		cout << "	PLACE  : " << path << endl;
		cout << "	REASON : '" << path << "' can not be opened." << endl;
		return false;
	}
	for (int row = Data::HEADER; row < table.size(); ++row) {
		resultDataFile << table[row][0];
		for (int i = 1; i < table[row].size(); ++i)
			resultDataFile << "," << table[row][i];
		resultDataFile << endl;
	}
	return true;
}

bool Data::generateLogFile(const string& path) {
	return threads.generateLogFile(path);
}

WorkThreads Data::threads;