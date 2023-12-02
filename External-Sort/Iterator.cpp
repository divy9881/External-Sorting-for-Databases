#include "Iterator.h"
#include "Sort.h"
//#include <experimental/filesystem>
#include <filesystem>
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

//namespace fs = std::experimental::filesystem;
namespace fs = std::filesystem;
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
bool Iterator::verifySortOrder(string device) {
    TRACE(ENABLE_TRACE);
    std::cout << "Inside sort order function" << std::endl;
    std::string directoryPath = "./" + device;
    std::string highestRunFilePath = findHighestRunFile(directoryPath);

    if (highestRunFilePath.empty()) {
        std::cerr << "No run files found in the directory: " << directoryPath << std::endl;
        return false;
    }

    std::ifstream infile(highestRunFilePath.c_str());
    if (!infile.is_open()) {
        std::cerr << "Unable to open file: " << highestRunFilePath << std::endl;
        return false;
    }

    const size_t chunk_size = 1024; // Define your desired chunk size
    char chunk[chunk_size];

    std::string prevFirstColumn;
    std::string currFirstColumn;

    // Read the first record
    if (!std::getline(infile, prevFirstColumn, ' ')) {
        std::cerr << "Error reading the first column of the first record" << std::endl;
        infile.close();
        return false;
    }

    while (infile.read(chunk, chunk_size)) {
        size_t bytes_read = infile.gcount();
        size_t i = 0;

        for (i = 0; i < bytes_read; ++i) {
            currFirstColumn += chunk[i];

            // Check for the column delimiter ' '
            if (chunk[i] == ' ') {
                // Compare the first column for sorting order
                cout << prevFirstColumn << endl;
                if (prevFirstColumn > currFirstColumn) {
                    std::cerr << "The sort order is not correct" << std::endl;
                    infile.close();
                    return false;
                }

                // Update prevFirstColumn for the next iteration
                prevFirstColumn = currFirstColumn;
                currFirstColumn.clear();
            }
        }
    }

    infile.close();
    return true;
}

        
//     }

//     infile.close();
//     cout << "The sort order is correct" << endl;
//     return true;
// }

// lluint StorageDevice::get_run_num_records(uint run)
// {
//     fstream runfile;
//     lluint count = 0;
//     string run_path = this->device_path + "/run_" + to_string(run);

//     const size_t chunk_size = 1024; // Define your desired chunk size
//     char chunk[chunk_size];
//     char *run_page = new char[ON_DISK_RECORD_SIZE + 1];

//     runfile.open(run_path, ios::in);

//     if (!runfile.is_open())
//     {
//         delete[] run_page;
//         return -1;
//     }

//     while (!runfile.eof())
//     {
//         runfile.read(chunk, chunk_size);
//         size_t bytes_read = runfile.gcount();

//         for (size_t i = 0; i < bytes_read; i += ON_DISK_RECORD_SIZE)
//         {
//             // Extract a record from the chunk
//             strncpy(run_page, &chunk[i], ON_DISK_RECORD_SIZE);
//             run_page[ON_DISK_RECORD_SIZE] = '\0'; // Null-terminate the record

//             // Validate the sort order or perform other processing here
//             // Note: Modify this part according to your actual record comparison logic

//             // Increment count if the record is valid
//             count++;
//         }
//     }

//     runfile.close();
//     delete[] run_page;

//     return count;
// }