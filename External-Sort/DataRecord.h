#pragma once

#include "defs.h"
#include "OffsetValueCode.h"

class DataRecord
{
public:
	DataRecord (lluint col1, lluint col2, lluint col3);
	DataRecord (const DataRecord& record);
	DataRecord ();
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
	bool operator>(const DataRecord& other) const;
	bool operator==(const DataRecord& other) const;
// private:
	lluint _record[3];
	suint ovc;
	string rel;
	int index; // index of the DataRecord in the list (required to maintain the ordering)
}; // class DataRecord
