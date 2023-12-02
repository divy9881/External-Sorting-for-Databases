#pragma once

#include "defs.h"
#include "databaseConfig.h"
#include "sortParams.h"

class DataRecord
{
public:
	DataRecord ();
	DataRecord (lluint col1, lluint col2, lluint col3, uint col_value_length);
	DataRecord (const DataRecord& record);
	~DataRecord ();
	void SetRecord (lluint col1, lluint col2, lluint col3, uint col_value_length);
	string GetRecord ();
	friend class OffsetValueCode;
	void print ();
	/**
	 * Check if the current DataRecord (in the tree) is a loser
	 * @param incoming_record The DataRecord object which is trying upward traversal.
	 * @return True if the current record loses (stays at the place),
	 *  False if the incoming record loses (The current record moves up the tree)
	*/
	bool is_smaller_int(DataRecord incoming_record);
	bool is_smaller_str(DataRecord incoming_record);
	/**
	 * Creates or updates the offset value code for the current data record
	 * when it loses.
	 * @param winner The datarecord that won at a position in the tree (We update the OVC for the loser)
	 * @return void
	 */
	void populate_ovc(DataRecord winner);
	bool operator<(const DataRecord& other) const;
	bool operator==(const DataRecord& other) const;
	void populate_ovc_int(lluint current, lluint winner_key);
	void populate_ovc_str(string current, string winner_key);
	// private:
	lluint _record[3];
	luint index; // index of the DataRecord in the list (required to maintain the ordering)
	uint ovc;
	char rel[256 + 1];
	uint col_value_length;
}; // class DataRecord

/*
 * @struct RecordList
 * Wrapper for the (list of) data records and the count of records in the list
 * Has two associated functions, pop_record and top_record
 */
typedef struct RecordList
{
	DataRecord *record_ptr = NULL;
	lluint record_count = 0;
} RecordList;

int comparator(const void *arg1, const void *arg2);
void InternalSort(RecordList *records);
