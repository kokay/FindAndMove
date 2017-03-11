#include <fstream>
#include <iostream>
#include "Configuration.hpp"

using namespace std;

const string Configuration::MOVE_FROM_DATA = "MOVE_FROM_DATA";
const string Configuration::MOVE_FROM_DATA_COMPARED_HEADER = "MOVE_FROM_DATA_COMPARED_HEADER";
const string Configuration::MOVE_TO_DATA = "MOVE_TO_DATA";
const string Configuration::MOVE_TO_DATA_COMPARED_HEADER = "MOVE_TO_DATA_COMPARED_HEADER";
const string Configuration::RESULT_DATA = "RESULT_DATA";
const string Configuration::LOG = "LOG";

const string Configuration::MOVE_FROM_DATA_START_EXTRA_IDX = "MOVE_FROM_DATA_START_EXTRA_IDX";
const string Configuration::ACCURACY = "ACCURACY";

bool Configuration::readConfFile(const string& confFilePath) {
	ifstream ifs(confFilePath, ifstream::in);
	if (!ifs.is_open()) {
		cout << "\nERROR" << endl;
		cout << "	PLACE  : " << confFilePath << endl;
		cout << "	REASON : '" << confFilePath << "' can not be opened." << endl;
		return false;
	}

	string line, type, value;
	while (getline(ifs, line)) {
		int idx = (int)line.find(" ");
		if (idx == string::npos || idx >= line.size()) {
			cout << "\nERROR" << endl;
			cout << "	PLACE  : " << confFilePath << endl;
			cout << "	REASON : '" << line << "' does not have a whitespace betweeen type and value or does not have a value" << endl;
			cout << "            Ex. MOVE_FROM_DATA_PATH E:data/MOVE_FROM_data_path.csv" << endl;
			return false;
		}

		type = line.substr(0, idx);
		auto it = paths.find(type);
		if (it != paths.end()) {
			it->second = line.substr(idx + 1);
		}
		else if (type == MOVE_FROM_DATA_START_EXTRA_IDX) {
			startExtraIdx = parseInt(line.substr(idx + 1));
		}
		else if (type == ACCURACY) {
			accuracy = parseInt(line.substr(idx + 1));
		}
		else {
			cout << "\nERROR" << endl;
			cout << "	PLACE  : " << confFilePath + " " + type << endl;
			cout << "	REASON : '" << type << "' does not exists." << endl;
			cout << "            Path Types are" << endl;
			for (auto& pathInfo : paths)
				cout << "                " << pathInfo.first << endl;
			cout << "                " << MOVE_FROM_DATA_START_EXTRA_IDX << endl;
			cout << "                " << ACCURACY << endl;
			return false;
		}
	}

	for (const auto& pathInfo : paths) {
		if (pathInfo.second.empty()) {
			cout << "ERROR" << endl;
			cout << "	PLACE  : " << confFilePath << " " << pathInfo.first << endl;
			cout << "	REASON : '" << pathInfo.first << "' does not have a path." << endl;
			cout << "            Please specific path for the path." << endl;
			return false;
		}
		else {
			cout << "  " << pathInfo.first << " : " << pathInfo.second << endl;
		}
	}
	if (startExtraIdx < 0) {
		cout << "ERROR" << endl;
		cout << " PLACE  : " << confFilePath << " " << MOVE_FROM_DATA_START_EXTRA_IDX << endl;
		cout << " REASON : '" << MOVE_FROM_DATA_START_EXTRA_IDX << "' does not have a value or the value is not rigth value." << endl;
		cout << "            Please assign a value greater than or equal to 0." << endl;
		return false;
	}
	else {
		cout << "  " << MOVE_FROM_DATA_START_EXTRA_IDX << " : " << startExtraIdx << endl;
	}
	if (accuracy < 0 || accuracy > 100) {
		cout << "ERROR" << endl;
		cout << " PLACE  : " << confFilePath << " " << ACCURACY << endl;
		cout << " REASON : '" << ACCURACY << "' does not have a value or the value is not rigth value." << endl;
		cout << "            Please assign a value greater than or equal to 0 and less than or equal to 100." << endl;
		return false;
	}
	else {
		cout << "  " << ACCURACY << " : " << accuracy << endl;
	}
	return true;
}

int Configuration::parseInt(const string& numStr) {
	try {
		return stoi(numStr);
	}
	catch (const invalid_argument&) {
		return -1;
	}
}