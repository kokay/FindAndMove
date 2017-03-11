#include <iostream>
#include "Configuration.hpp"
#include "Data.hpp"

using namespace std;

int NUM_CORES = (thread::hardware_concurrency() == 0) ? 2
	: thread::hardware_concurrency();

int main(int argc, char** argv) {
	string root(argv[0]);
	string confFile = root.substr(0, root.find_last_of("\\")) + "\\conf.txt";

	Configuration conf;
	cout << "Reading the configuration file..." << endl;
	if (!conf.readConfFile(confFile)) {
		getchar();
		return EXIT_FAILURE;
	}
	cout << "Done" << endl << endl;

	Data moveFromData, moveToData;
	cout << "Preparing for MOVE_FROM Data... : ";
	if (!moveFromData.prepare(conf.getPath(Configuration::MOVE_FROM_DATA),
		conf.getPath(Configuration::MOVE_FROM_DATA_COMPARED_HEADER))) {
		getchar();
		return EXIT_FAILURE;
	}
	cout << "Done" << endl << endl;

	cout << "Preparing for MOVE_TO Data... : ";
	if (!moveToData.prepare(conf.getPath(Configuration::MOVE_TO_DATA),
		conf.getPath(Configuration::MOVE_TO_DATA_COMPARED_HEADER))) {
		getchar();
		return EXIT_FAILURE;
	}
	moveToData.addHeaderExtra(moveFromData.getHeader(), conf.getStartExtraIdx());
	cout << "Done" << endl << endl;

	cout << "Finding Similar Row From MOVE_FROM_DATA for All Row in MOVE_TO_DATA (" << NUM_CORES << " Threads)..." << endl;
	moveToData.findSimilerRowForAll(moveFromData, conf.getAccuracy(),
		conf.getStartExtraIdx(), NUM_CORES);
	cout << "Done" << endl << endl;

	cout << "Writing the Result Data File... : ";
	if (!moveToData.generateResultDataFile(conf.getPath(Configuration::RESULT_DATA))) {
		getchar();
		return EXIT_FAILURE;
	}
	cout << "Done" << endl << endl;

	cout << "Writing the Log File... : ";
	if (!Data::generateLogFile(conf.getPath(Configuration::LOG))) {
		getchar();
		return EXIT_FAILURE;
	}
	cout << "Done" << endl << endl;

	cout << "All Done" << endl;
	getchar();
}