#include "Iterator.h"
#include "Sort.h"
//#include <experimental/filesystem>
// #include <filesystem>
#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
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

//namespace fs = std::experimental::filesystem;
// namespace fs = std::filesystem;
// This function is used the find the highest run file generated
// Since the number of runs generated are dynamically changed
// // A separate function is used to find the file accordingly
// std::string Iterator::findHighestRunFile(const std::string& directoryPath) {
//     int highestRunNumber = -1;
//     std::string highestRunFilePath;

//     for (const auto& entry : fs::directory_iterator(directoryPath)) {
//         if (entry.is_regular_file()) {
//             std::string fileName = entry.path().filename().string();

//             if (fileName.compare(0, 4, "run_") == 0) {
//                 int runNumber = std::atoi(fileName.substr(4).c_str());
//                 if (runNumber > highestRunNumber) {
//                     highestRunNumber = runNumber;
//                     highestRunFilePath = entry.path().string();
//                 }
//             }
//         }
//     }

//     return highestRunFilePath;
// }

// This function will take the expected records
// from the input and verify it against the records 
// that are spilled into the files.
// If both the values match, return true or else false
// This function will take time, thinking maybe we can avarage each runs
// Parameters - Accepts device type and expected number from the input command
// bool Iterator::verifyNumRecords(string device, int numberRecords) {
//     TRACE(ENABLE_TRACE);
    
// 	std::string directoryPath = "./"+device;  // Replace with the actual path to your device directory
//     std::string highestRunFilePath = findHighestRunFile(directoryPath);

//     if (highestRunFilePath.empty()) {
//         std::cerr << "No run files found in the directory: " << directoryPath << std::endl;
//         return false;
//     }
//     int count = 0;
//     string run_path = highestRunFilePath; // Replace with the actual path

//     ifstream infile(run_path.c_str());

//     if (!infile.is_open()) {
//         cerr << "Unable to open file: " << run_path << endl;
//         return false;
//     }

//     string line;
//     while (getline(infile, line)) {
//         cout << count << endl;
//         // Assuming each line represents a record
//         ++count;
//     }

//     infile.close();

//     // Compare the counted records with the expected numberRecords
//     if (count == numberRecords) {
//         cout << "Number of records matches the expected count." << endl;
//         return true;
//     } else {
//         cout << "Mismatch: Expected " << numberRecords << " records, but found " << count << " records." << endl;
//         return false;
//     }
// }

//This function will verify the sortorder
//It will open the file containin the sorted order records
//Parse through each line and will do a "<" and return the boolean accordingly
//Parameter - Accepts device type

constexpr size_t CHUNK_SIZE = 1024;

std::pair<bool, llint> processChunk(const std::vector<std::string>& lines, llint& recordCount) {
    int previous = std::numeric_limits<int>::lowest();  // Initialize previous to a very small value

    for (const auto& line : lines) {
        std::istringstream iss(line);
        int firstColumnValue;

        // Read and check the first value in each line
        if (iss >> firstColumnValue) {
            // Check if the value in the first column is in ascending order
            if (firstColumnValue >= previous) {
                previous = firstColumnValue;
                recordCount += 1; // only increase the recourd count if the data is in increasing order
            } else {
                std::cerr << "Invalid sort order in the row." << std::endl;
                return std::make_pair(false, recordCount);
            }
        } else {
            std::cerr << "Failed to read the first column value." << std::endl;
            return std::make_pair(false, recordCount);
        }

        // Additional check for data corruption have to test
        // std::string restOfLine;
        // if (std::getline(iss >> std::ws, restOfLine)) {
        //     if (!restOfLine.empty()) {
        //         std::cerr << "Data corruption detected in the row." << std::endl;
        //         return std::make_pair(false, recordCount);;
        //     }
        // }
    }

    return std::make_pair(true, recordCount);
}

std::pair<bool, llint> Iterator::verifySortOrder() {
    TRACE(ENABLE_TRACE);

    std::ifstream file("input/table");

    if (!file.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return std::make_pair(false, 0);
    }

    llint recordCount = 0;
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(file, line)) {
        lines.push_back(line);

        if (lines.size() * line.size() >= CHUNK_SIZE) {
            // Process the chunk
            auto result = processChunk(lines, recordCount);

            if (!result.first) {
                return result;
            }

            // Clear the processed lines
            lines.clear();
        }
    }

    // Process the remaining lines in the last chunk
    if (!lines.empty()) {
        auto result = processChunk(lines, recordCount);

        if (!result.first) {
            // Handle the error as needed
            return result;
        }
    }

    // Close the file
    file.close();

    return std::make_pair(true, recordCount);
}