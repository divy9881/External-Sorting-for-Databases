#pragma once

#include "defs.h"
#include "databaseConfig.h"
#include "sortParams.h"
#include <list>

class DataRecord
{
public:
	DataRecord ();
	DataRecord (lluint col1, lluint col2, lluint col3);
	DataRecord (const DataRecord& record);
	~DataRecord ();
	void SetRecord (lluint col1, lluint col2, lluint col3);
	string GetRecord ();
	friend class OffsetValueCode;
	void print ();
	/**
	 * Check if the current DataRecord (in the tree) is a loser
	 * @param incoming_record The DataRecord object which is trying upward traversal.
	 * @return True if the current record loses (stays at the place),
	 *  False if the incoming record loses (The current record moves up the tree)
	 */
	bool is_smaller_int(const DataRecord incoming_record) const;
	bool is_smaller_str(const DataRecord incoming_record) const;
	/**
	 * Creates or updates the offset value code for the current data record
	 * when it loses.
	 * @param winner The datarecord that won at a position in the tree (We update the OVC for the loser)
	 * @return void
	 */
	void populate_ovc_int(DataRecord winner);
	void populate_ovc_str(DataRecord winner);
	bool operator<(const DataRecord& other) const;
	bool operator==(const DataRecord& other) const;
	bool operator>(const DataRecord& other) const;
	// private:
	lluint _record[3];
	luint index; // index of the DataRecord in the list (required to maintain the ordering)
	uint ovc;
	char rel[NUM_CHARS_COL_VALUE + 1];
}; // class DataRecord

/*
 * @struct RecordList
 * Wrapper for the (list of) data records and the count of records in the list
 * Has two associated functions, pop_record and top_record
 */
typedef struct RecordList
{
	std::list<DataRecord> record_ptr;
	lluint record_count = 0;
} RecordList;

int comparator(const void *arg1, const void *arg2);
void InternalSort(RecordList *records);
