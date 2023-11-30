#include "Iterator.h"
#include "Sort.h"
#include <experimental/filesystem>
//#include <filesystem>
#include <string>
#include <iostream>
#include <cstdlib>
Plan::Plan ()
{
	TRACE(ENABLE_TRACE);
} // Plan::Plan

Plan::~Plan ()
{
	TRACE(ENABLE_TRACE);
} // Plan::~Plan

Iterator::Iterator () : _count (0)
{
	TRACE(ENABLE_TRACE);
} // Iterator::Iterator

Iterator::~Iterator ()
{
	TRACE(ENABLE_TRACE);
} // Iterator::~Iterator

void Iterator::run ()
{
	TRACE(ENABLE_TRACE);

	while (next ())  ++ _count;

	traceprintf ("entire plan produced %lu rows\n",
			(unsigned long) _count);
} // Iterator::run

namespace fs = std::experimental::filesystem;
//namespace fs = std::filesystem;
// This function is used the find the highest run file generated
// Since the number of runs generated are dynamically changed
// A separate function is used to find the file accordingly
std::string Iterator::findHighestRunFile(const std::string& directoryPath) {
    int highestRunNumber = -1;
    std::string highestRunFilePath;

    for (const auto& entry : fs::directory_iterator(directoryPath)) {
        if (entry.is_regular_file()) {
            std::string fileName = entry.path().filename().string();

            if (fileName.compare(0, 4, "run_") == 0) {
                int runNumber = std::atoi(fileName.substr(4).c_str());
                if (runNumber > highestRunNumber) {
                    highestRunNumber = runNumber;
                    highestRunFilePath = entry.path().string();
                }
            }
        }
    }

    return highestRunFilePath;
}

// This function will take the expected records
// from the input and verify it against the records 
// that are spilled into the files.
// If both the values match, return true or else false
// This function will take time, thinking maybe we can avarage each runs
// Parameters - Accepts device type and expected number from the input command
bool Iterator::verifyNumRecords(string device, int numberRecords) {
    TRACE(ENABLE_TRACE);
	std::string directoryPath = "./"+device;  // Replace with the actual path to your device directory
    std::string highestRunFilePath = findHighestRunFile(directoryPath);

    if (highestRunFilePath.empty()) {
        std::cerr << "No run files found in the directory: " << directoryPath << std::endl;
        return false;
    }
    int count = 0;
    string run_path = highestRunFilePath; // Replace with the actual path

    ifstream infile(run_path.c_str());

    if (!infile.is_open()) {
        cerr << "Unable to open file: " << run_path << endl;
        return false;
    }

    string line;
    while (getline(infile, line)) {
        // Assuming each line represents a record
        ++count;
    }

    infile.close();

    // Compare the counted records with the expected numberRecords
    if (count == numberRecords) {
        cout << "Number of records matches the expected count." << endl;
        return true;
    } else {
        cout << "Mismatch: Expected " << numberRecords << " records, but found " << count << " records." << endl;
        return false;
    }
}

//This function will verify the sortorder
//It will open the file containin the sorted order records
//Parse through each line and will do a "<" and return the boolean accordingly
//Parameter - Accepts device type
bool Iterator::verifySortOrder(string device) {
    TRACE(ENABLE_TRACE);
	// opening the directory path for the individual file
	std::string directoryPath = "./"+device;  // Replace with the actual path to your device directory
    std::string highestRunFilePath = findHighestRunFile(directoryPath);

    if (highestRunFilePath.empty()) {
        std::cerr << "No run files found in the directory: " << directoryPath << std::endl;
        return false;
    }
	// opening the last run file 
    string run_path = highestRunFilePath; // Replace with the actual path
    ifstream infile(run_path.c_str());

    if (!infile.is_open()) {
        cerr << "Unable to open file: " << run_path << endl;
        return false;
    }

    string prevLine;
    string currLine;
	// Comparision logic
    while (getline(infile, currLine)) {
        if (!currLine.empty() && prevLine > currLine) {
            cerr << "The sort order is not correct" << endl;
            infile.close();  // Close the file before returning
            return false;
        }
        prevLine = currLine;
    }

    infile.close();
    cout << "The sort order is correct" << endl;
    return true;
}
