#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <string>a
#include <map>

using namespace std;

class Configuration {
public:
	const static string MOVE_FROM_DATA;
	const static string MOVE_FROM_DATA_COMPARED_HEADER;
	const static string MOVE_TO_DATA;
	const static string MOVE_TO_DATA_COMPARED_HEADER;
	const static string RESULT_DATA;
	const static string LOG;

	const static string MOVE_FROM_DATA_START_EXTRA_IDX;
	const static string ACCURACY;

	bool readConfFile(const string& confFilePath);
	const string& getPath(const string& type) { return paths[type]; };
	const int getStartExtraIdx() const { return startExtraIdx; };
	const int getAccuracy() const { return accuracy; };

private:
	int parseInt(const string& numStr);
	map<const string, string> Configuration::paths = {
		{ MOVE_FROM_DATA, "" },
		{ MOVE_FROM_DATA_COMPARED_HEADER, "" },
		{ MOVE_TO_DATA, "" },
		{ MOVE_TO_DATA_COMPARED_HEADER, "" },
		{ RESULT_DATA, "" },
		{ LOG, "" }
	};
	int startExtraIdx = -1;
	int accuracy = -1;
};

#endif // CONFIGURATION_HPP
