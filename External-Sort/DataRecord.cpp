#include "DataRecord.h"

DataRecord::DataRecord() {}

DataRecord::DataRecord (lluint col1, lluint col2, lluint col3)
{
	this->_record[0] = col1;
	this->_record[1] = col2;
	this->_record[2] = col3;
	this->ovc = 0;
	this->rel = "";
	TRACE (false);
} // DataRecord::DataRecord (lluint col1, lluint col2, lluint col3)

DataRecord::DataRecord (const DataRecord& record)
{
	this->_record[0] = record._record[0];
	this->_record[1] = record._record[1];
	this->_record[2] = record._record[2];
	this->ovc = record.ovc;
	this->rel = record.rel;
	TRACE (false);
} // DataRecord::DataRecord (const DataRecord& record)

DataRecord::~DataRecord ()
{
	TRACE (false);
} // DataRecord::~DataRecord

void DataRecord::SetRecord (lluint col1, lluint col2, lluint col3)
{
	this->_record[0] = col1;
	this->_record[1] = col2;
	this->_record[2] = col3;
	this->ovc = 0;
	this->rel = "";
} // DataRecord::SetRecord()

string DataRecord::GetRecord ()
{
	uint diff, count = 0;
	string record = "";
	string col1_value, col2_value, col3_value;

	diff = NUM_CHARS_COL_VALUE - to_string(this->_record[0]).length();
	count = 0;
	while (count < diff) {
		col1_value += "0";
		count += 1;
	}
	col1_value += to_string(this->_record[0]);

	diff = NUM_CHARS_COL_VALUE - to_string(this->_record[1]).length();
	count = 0;
	while (count < diff) {
		col2_value += "0";
		count += 1;
	}
	col2_value += to_string(this->_record[1]);

	diff = NUM_CHARS_COL_VALUE - to_string(this->_record[2]).length();
	count = 0;
	while (count < diff)
	{
		col3_value += "0";
		count += 1;
	}
	col3_value += to_string(this->_record[2]);

	record = col1_value + COLUMN_DELIMITER + col2_value + COLUMN_DELIMITER + col3_value;

	return record;
} // DataRecord::GetRecord()

void DataRecord::print ()
{
    cout<<this->_record[0]<<" "<<this->_record[1]<<" "<<this->_record[2]<<" ";
	cout << "OVC: " << this->ovc;
	cout << " Rel: " << this->rel << endl;
	TRACE (false);
} // DataRecord::print

// For integers, OVC would not matter too much (it is faster than string comparison)
bool DataRecord::is_smaller_int (DataRecord incoming_record)
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

void DataRecord::populate_ovc (DataRecord winner)
{
	// TODO: Update this when we shift to strings
	this->populate_ovc_int(this->_record[0], winner._record[0]);
}

bool DataRecord::operator< (const DataRecord& other) const
{
	return _record[0] < other._record[0];
}

bool DataRecord::operator== (const DataRecord& other) const 
{
	return equal(begin(_record), end(_record), begin(other._record));
}

// It is called only when the current record loses to the incoming
// (incoming record will be shorter/have lower value)
void DataRecord::populate_ovc_int(lluint current, lluint winner)
{
	string current_record = to_string(current);
	string winner_record = to_string(winner);

	this->populate_ovc_str(current_record, winner_record);
}

void DataRecord::populate_ovc_str(string current, string winner)
{
	int current_length = current.length();
	int winner_length = winner.length();
	int arity = 0;

	if (current_length > winner_length)
	{
		int num_of_zeroes = current_length - winner_length;
		string zeroes = "";

		for (int ii = 0; ii < num_of_zeroes; ii++)
		{
			zeroes += "0";
		}
		winner = zeroes + winner;
	}
	else
	{
		int num_of_zeroes = winner_length - current_length;
		string zeroes = "";

		for (int ii = 0; ii < num_of_zeroes; ii++)
		{
			zeroes += "0";
		}
		current = zeroes + current;
	}

	arity = winner.length();

	// Traverse over winner record length (it will be shorter/lower value)
	for (lluint ii = 0; ii < winner.length(); ii++)
	{
		if (current[ii] == winner[ii])
		{
			continue;
		}
		else
		{
			this->ovc = (arity - ii) * OVC_DOMAIN + (current[ii] - '0');
			this->rel = winner;
			break;
		}
	}
}

int comparator(const void *arg1, const void *arg2)
{
	if (*(DataRecord *)arg1 < *(DataRecord *)arg2)
	{
		return -1;
	}
	else if (*(DataRecord *)arg1 == *(DataRecord *)arg2)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

void InternalSort(RecordList* records)
{
	qsort((void *)records->record_ptr, (size_t)records->record_count, sizeof(DataRecord), comparator);
}
