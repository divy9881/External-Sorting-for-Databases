#pragma once

#include "defs.h"
#include "OffsetValueCode.h"

class DataRecord
{
public:
	DataRecord (string col1, string col2, string col3);
	DataRecord (const DataRecord& record);
	DataRecord ();
	~DataRecord ();
	void SetRecord(string col1, string col2, string col3);
	friend class OffsetValueCode;
	void print();
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

// private:
	string _record[3];
	OffsetValueCode ov_code;
	int index; // index of the DataRecord in the list (required to maintain the ordering)
}; // class DataRecord
