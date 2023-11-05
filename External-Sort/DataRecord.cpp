#include "DataRecord.h"

DataRecord::DataRecord() {}

DataRecord::DataRecord (int col1, int col2, int col3)
{
	this->_record[0] = col1;
	this->_record[1] = col2;
	this->_record[2] = col3;
	TRACE (false);
} // DataRecord::DataRecord (int col1, int col2, int col3)

DataRecord::DataRecord (const DataRecord& record)
{
	this->_record[0] = record._record[0];
	this->_record[1] = record._record[1];
	this->_record[2] = record._record[2];
	TRACE (false);
} // DataRecord::DataRecord (const DataRecord& record)

DataRecord::~DataRecord ()
{
	this->_record[0] = INT_MIN;
	this->_record[1] = INT_MIN;
	this->_record[2] = INT_MIN;
	TRACE (false);
} // DataRecord::~DataRecord

void DataRecord::SetRecord(int col1, int col2, int col3)
{
	this->_record[0] = col1;
	this->_record[1] = col2;
	this->_record[2] = col3;
} // DataRecord::SetRecord()

void DataRecord::print ()
{
	cout<<this->_record[0]<<this->_record[1]<<this->_record[2]<<endl;
	this->ov_code.print();
	TRACE (false);
} // DataRecord::print

// For integers, OVC would not matter too much (it is faster than string comparison)
bool DataRecord::is_smaller_int(DataRecord incoming_record)
{
	// TODO How to preserve order here?
	// TODO Add a way to sort as per the other two columns as well
	if (incoming_record._record[0] >= this->_record[0]) {
		return true;
	}
	return false;
}

/*
bool DataRecord::is_smaller_str(DataRecord incoming_record)
{
	// If the offsets are not there (first pass)
	if ((this->ov_code.offset == (-1)) || (incoming_record.ov_code.offset == (-1))) {
		// Compare character by character, for the first pass, we will generate the OVC after this.
		int min_size = incoming_record._record[0].length() < this->_record[0].length() ? incoming_record._record[0].length(): this._record[0].length();
		int ii = 0;
		while (++ii < min_size) {
			if (this->_record[0][ii] != incoming_record._record[0][ii]) {
				return this->_record[0][ii] < incoming_record._record[0][ii];
			}
		}
		return false;
	} else if (this->ov_code.offset != incoming_record.ov_code.offset) {
		// Larger offset == smaller data record
		return this->ov_code.offset > incoming_record.ov_code.offset;
	} else {
		// If the offsets are same, check with the values
		if (this->ov_code.value != incoming_record.ov_code.value) {
			// Smaller value at same offset == smaller data record
			return this->ov_code.value < incoming_record.ov_code.value;
		} else {
			// Offset and value both are same, check for the next set of
			// characters to determine which record is smaller
			// (the values will be in relation with the previous winner)
			int incoming_record_offset = incoming_record.ov_code.offset;
			// Since the offsets are same for both the records,
			// check from the next offset value for both
			while (++incoming_record_offset < incoming_record._record[0].length()) {
				if (this->_record[0][incoming_record_offset] != incoming_record._record[0][incoming_record_offset]) {
					return this->_record[0][incoming_record_offset] < incoming_record._record[0][incoming_record_offset];
				}
			}
			return false;
		}
	}
	return false;
}
*/

void DataRecord::populate_ovc(DataRecord winner)
{
	// TODO: Update this when we shift to strings
	this->ov_code.populate_ovc_int(this->_record[0], winner._record[0]);
}

bool DataRecord::operator<(const DataRecord& other) const
{
	return _record[0] < other._record[0];
}

bool DataRecord::operator==(const DataRecord& other) const 
{
	return equal(begin(_record), end(_record), begin(other._record));
}
