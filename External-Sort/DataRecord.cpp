#include "DataRecord.h"

DataRecord::DataRecord() {
	this->_record[0] = this->_record[1] = this->_record[2] = INT_MIN;
	this->index = -1;
	this->next = NULL;
	this->ovc = 0;
	this->rel = '\0';
}

DataRecord::DataRecord (int col1, int col2, int col3)
{
	this->_record[0] = col1;
	this->_record[1] = col2;
	this->_record[2] = col3;
	this->index = -1;
	this->next = NULL;
	this->ovc = 0;
	this->rel = '\0';
	TRACE (false);
} // DataRecord::DataRecord (int col1, int col2, int col3)

DataRecord::DataRecord (const DataRecord& record)
{
	this->_record[0] = record._record[0];
	this->_record[1] = record._record[1];
	this->_record[2] = record._record[2];
	this->next = NULL;
	this->index = -1;
	this->ovc = 0;
	this->rel = '\0';
	TRACE (false);
} // DataRecord::DataRecord (const DataRecord& record)

DataRecord::~DataRecord ()
{
	this->_record[0] = INT_MIN;
	this->_record[1] = INT_MIN;
	this->_record[2] = INT_MIN;
	this->next = NULL;
	this->ovc = 0;
	this->rel = '\0';
	TRACE (false);
} // DataRecord::~DataRecord

void DataRecord::SetRecord(int col1, int col2, int col3)
{
	this->_record[0] = col1;
	this->_record[1] = col2;
	this->_record[2] = col3;
	this->ovc = 0;
	this->rel = '\0';
} // DataRecord::SetRecord()

void DataRecord::print ()
{
    cout<<this->_record[0]<<" "<<this->_record[1]<<" "<<this->_record[2]<<" ";
	if (this->ovc == 0) {
		cout<<"{:}"<<endl;
	} else {
		cout<<"{"<<this->ovc<<":"<<this->rel<<"}"<<endl;
	}
    TRACE (false);
} // DataRecord::print

bool DataRecord::is_smaller_int(const DataRecord incoming_record) const
{
	// TODO How to preserve order here?
	if (incoming_record._record[0] > this->_record[0]) {
		return true;
	}
	return false;
}


// bool DataRecord::is_smaller_str(DataRecord *incoming_record)
// {
// 	int min_size = incoming_record->_record[0].length() < this->_record[0].length() ? incoming_record->_record[0].length(): this->_record[0].length();
// 	// If the offsets are not there (first pass)
// 	if ((this->ovc == 0) || (incoming_record->ovc == 0)) {
// 		// Compare character by character, for the first pass, we will generate the OVC after this.
// 		int ii = 0;
// 		while (++ii < min_size) {
// 			if (this->_record[0][ii] != incoming_record->_record[0][ii]) {
// 				return this->_record[0][ii] < incoming_record->_record[0][ii];
// 			}
// 		}
// 		return false;
// 	} else if (this->ovc != incoming_record->ovc) {
// 		// Smaller value of OVC == smaller record
// 		return this->ovc < incoming_record->ovc;
// 	} else {
// 		// If the offsets are same, we need to iterate over the values
		// // Since the offsets are same for both the records,
		// // check from the next offset value for both
		// int record_offset = 0;
		// while (++record_offset < incoming_record->_record[0].length()) {
		// 	if (this->_record[0][record_offset] != incoming_record->_record[0][record_offset]) {
		// 		return this->_record[0][record_offset] < incoming_record->_record[0][record_offset];
		// 	}
		// }
		// return false;
// 	}
// 	return false;
// }

void DataRecord::populate_ovc_str(DataRecord winner) {
	string current_record = to_string(this->_record[0]);
	string winner_record = to_string(winner._record[0]);

	int current_length = current_record.length();
	int winner_length = winner_record.length();
	int arity = 0;

	if (current_length > winner_length) {
		int num_of_zeroes = current_length - winner_length;
		string zeroes = "";

		for (int ii = 0; ii < num_of_zeroes; ii++) {
			zeroes += "0";
		}
		winner_record = zeroes + winner_record;
	} else {
		int num_of_zeroes = winner_length - current_length;
		string zeroes = "";

		for (int ii = 0; ii < num_of_zeroes; ii++) {
			zeroes += "0";
		}
		current_record = zeroes + current_record;
	}

	arity = winner_record.length();

	// Traverse over winner record length (it will be shorter/lower value)
	for (lluint ii = 0; ii < winner_record.length(); ii++) {
		if (current_record[ii] == winner_record[ii]) {
			continue;
		} else {
			this->ovc = (arity - ii) * OVC_DOMAIN + (current_record[ii] - '0');
			this->rel = winner_record;
			break;
		}
	}
}

void DataRecord::populate_ovc_int(DataRecord winner)
{
	// TODO: Update this when we shift to strings
	this->populate_ovc_str(winner);
}

bool DataRecord::operator<(const DataRecord& other) const
{	
	if (this->ovc < other.ovc)
		return true;
	return false;
}

bool DataRecord::operator>(const DataRecord& other) const
{	
	if (this->ovc > other.ovc)
		return true;
	return false;
}

bool DataRecord::operator==(const DataRecord& other) const 
{
	return equal(begin(_record), end(_record), begin(other._record));
}
