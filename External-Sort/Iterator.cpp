#include "Iterator.h"
#include "Sort.h"

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

constexpr size_t CHUNK_SIZE = 1024;

/*
 * This function will verify the sortorder
 * It will open the file containin the sorted order records
 * Parse through each line and will do a "<" and return the boolean accordingly
 * Parameter - Accepts device type
 */
template <typename T>
bool process_chunk(const vector<string> &lines)
{
    T previous;

    for (const auto& line : lines) {
        istringstream iss(line);
        T first_column_value;

        // Read and check the first value in each line
        if (iss >> first_column_value) {
            // Check if the value in the first column is in ascending order
            if (first_column_value >= previous) {
                previous = first_column_value;
            } else {
                cerr << "Invalid sort order in the row." << endl;
                return false;
            }
        } else {
            cerr << "Failed to read the first column value." << endl;
            return false;
        }
    }

    return true;
}

pair<bool, lluint> Iterator::verify_sort_order() {
    TRACE(ENABLE_TRACE);

    ifstream file("output/table");

    if (!file.is_open()) {
        cerr << "Failed to open the file." << endl;
        return make_pair(false, 0);
    }

    lluint record_count = 0;
    vector<string> lines;
    string line;

    while (getline(file, line)) {\
        record_count += 1;
        lines.push_back(line);

        if (lines.size() * line.size() >= CHUNK_SIZE) {
            // Process the chunk
            bool result = process_chunk<string>(lines);

            if (!result) {
                return make_pair(result, record_count);
            }

            // Clear the processed lines
            lines.clear();
        }
    }

    // Process the remaining lines in the last chunk
    if (!lines.empty()) {
        bool result = process_chunk<string>(lines);
        if (!result) {
            // Handle the error as needed
            return make_pair(result, record_count);
        }
    }

    // Close the file
    file.close();

    return make_pair(true, record_count);
}