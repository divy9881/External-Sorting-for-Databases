#include "DataRecord.h"

extern SortTrace trace;

#define GET_LENGTH(record_start, iterator, length_measure) length_measure=0;\
		 iterator = record_start;\
       	while (*iterator != '\0') {\
			length_measure++;\
			iterator++;\
       	}

DataRecord::DataRecord()
{
	this->ovc = 0;
	this->rel = '\0';
	TRACE (false);
}

DataRecord::DataRecord(string col1, string col2, string col3, uint col_value_length)
{
	strncpy(this->_record[0], col1.c_str(), col_value_length);
	strncpy(this->_record[1], col2.c_str(), col_value_length);
	strncpy(this->_record[2], col3.c_str(), col_value_length);
	this->ovc = 0;
	this->col_value_length = col_value_length;
	this->rel = '\0';
	TRACE (false);
} // DataRecord::DataRecord (lluint col1, lluint col2, lluint col3)

DataRecord::DataRecord (const DataRecord& record)
{
	strncpy(this->_record[0], record._record[0], record.col_value_length);
	strncpy(this->_record[1], record._record[1], record.col_value_length);
	strncpy(this->_record[2], record._record[2], record.col_value_length);
	this->ovc = record.ovc;
	this->col_value_length = record.col_value_length;
	// strcpy(this->rel, record.rel);
	TRACE (false);
} // DataRecord::DataRecord (const DataRecord& record)

DataRecord::~DataRecord ()
{
	TRACE (false);
} // DataRecord::~DataRecord

void DataRecord::SetRecord (string col1, string col2, string col3, uint col_value_length)
{
	this->col_value_length = col_value_length;
	strncpy(this->_record[0], col1.c_str(), col_value_length);
	strncpy(this->_record[1], col2.c_str(), col_value_length);
	strncpy(this->_record[2], col3.c_str(), col_value_length);

	this->ovc = 0;
} // DataRecord::SetRecord()

string DataRecord::GetRecord ()
{
	// TODO Add Append by 0 logic
	uint diff, count = 0;
	string record = "";
	string col1_value = "", col2_value = "", col3_value = "";
	int length = 0 ; char *iter = NULL;
	GET_LENGTH(this->_record[0], iter, length);

	diff = this->col_value_length - length;
	count = 0;
	while (count < diff) {
		col1_value += "0";
		count += 1;
	}
	col1_value += this->_record[0];

	iter = NULL; length = 0;
	GET_LENGTH(this->_record[1], iter, length);
	diff = this->col_value_length - length;
	count = 0;
	while (count < diff) {
		col2_value += "0";
		count += 1;
	}
	col2_value += this->_record[1];

	iter = NULL; length = 0;
	GET_LENGTH(this->_record[1], iter, length);
	diff = this->col_value_length - length;
	count = 0;
	while (count < diff)
	{
		col3_value += "0";
		count += 1;
	}
	col3_value += this->_record[2];

	record = col1_value + STORAGE_COLUMN_DELIMITER + col2_value + STORAGE_COLUMN_DELIMITER + col3_value;

	return record;
} // DataRecord::GetRecord()

void DataRecord::print ()
{
	printf("%.5s %.5s %.5s", this->_record[0], this->_record[1], this->_record[2]);
    // cout<<this->_record[0].<<" "<<this->_record[1]<<" "<<this->_record[2]<<" ";
	if (this->ovc == 0) {
		cout<<"{:}"<<endl<<flush;
	} else {
		cout<<"{"<<this->ovc<<":"<<this->rel<<"}"<<endl<<flush;
	}
	TRACE (false);
} // DataRecord::print

// For integers, OVC would not matter too much (it is faster than string comparison)
bool DataRecord::is_smaller_int (const DataRecord incoming_record) const
{
	// TODO How to preserve order here?
	// TODO Add a way to sort as per the other two columns as well
	if (incoming_record._record[0] > this->_record[0]) {
		return true;
	}
	return false;
}

bool DataRecord::is_smaller_str(const DataRecord incoming_record) const
{
	int incoming_length = 0 ; char const *incoming_iter = NULL;
	int current_length = 0 ; char const *current_iter = NULL;
	GET_LENGTH(this->_record[0], current_iter, current_length);
	GET_LENGTH(incoming_record._record[0], incoming_iter, incoming_length);
	// If the offsets are not there (first pass)
	if ((this->ovc == (0)) || (incoming_record.ovc == (0))) {
		// Compare character by character, for the first pass, we will generate the OVC after this.
		int min_size = incoming_length < current_length ? incoming_length: current_length;
		int ii = 0;
		while (ii < min_size) {
			if (this->_record[0][ii] != incoming_record._record[0][ii]) {
#if DEBUG_PRINT
				cout << "Current is "<< this->_record[0][ii] <<" AND the incoming is "<< incoming_record._record[0][ii] <<endl;
#endif
				return this->_record[0][ii] < incoming_record._record[0][ii];
			}
			ii++;
		}
		return false;
	} else {
		if (this->ovc != incoming_record.ovc) {
			// Larger offset == smaller data record
			return this->ovc > incoming_record.ovc;
		} else {
			// If the offsets are same, check with the values
			if (this->rel != incoming_record.rel) {
				// Smaller value at same offset == smaller data record
				return this->rel < incoming_record.rel;
			} else {
				// Offset and value both are same, check for the next set of
				// characters to determine which record is smaller
				// (the values will be in relation with the previous winner)
				int incoming_record_offset = ((int) incoming_record.ovc % OVC_DOMAIN) - 1;
				if (incoming_record_offset < 0) incoming_record_offset = 0;
				// Since the offsets are same for both the records,
				// check from the next offset value for both
				while (incoming_record_offset < incoming_length) {
					if (this->_record[0][incoming_record_offset] != incoming_record._record[0][incoming_record_offset]) {
						return this->_record[0][incoming_record_offset] < incoming_record._record[0][incoming_record_offset];
					}
					incoming_record_offset++;
				}
				return false;
			}
		}
	}
	return false;
}

void DataRecord::populate_ovc_int (DataRecord winner)
{
	// TODO: Update this when we shift to strings
	this->populate_ovc_str(winner);
}

bool DataRecord::operator< (const DataRecord& other) const
{
	if (this->ovc < other.ovc){
		return true;
	}
	return this->_record[0] < other._record[0];
}

bool DataRecord::operator>(const DataRecord& other) const
{	
	if (this->ovc > other.ovc) {
		return true;
	}
	return this->_record[0] > other._record[0];
}

bool DataRecord::operator== (const DataRecord& other) const 
{
	return equal(begin(_record), end(_record), begin(other._record));
}


void DataRecord::populate_ovc_str(DataRecord winner)
{
	string current_record = this->_record[0];
	string winner_record = winner._record[0];
#if DEBUG_PRINT
	cout<<current_record<< "  " << winner_record<<endl;
#endif
	int current_length = current_record.length();
	int winner_length = winner_record.length();
	int arity = 0;

	if (current_length > winner_length)
	{
		int num_of_zeroes = current_length - winner_length;
		string zeroes = "";

		for (int ii = 0; ii < num_of_zeroes; ii++)
		{
			zeroes += "0";
		}
		winner_record = zeroes + winner_record;
	}
	else
	{
		int num_of_zeroes = winner_length - current_length;
		string zeroes = "";

		for (int ii = 0; ii < num_of_zeroes; ii++)
		{
			zeroes += "0";
		}
		current_record = zeroes + current_record;
	}

	arity = winner_record.length();

	// Traverse over winner record length (it will be shorter/lower value)
	for (lluint ii = 0; ii < winner_record.length(); ii++)
	{
		if (current_record[ii] == winner_record[ii])
		{
			continue;
		}
		else
		{
			this->ovc = (arity - ii) * OVC_DOMAIN + (current_record[ii] - '0');
			this->rel = winner_record[ii];
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

bool comparator(const DataRecord& first, const DataRecord& second) {
	return (first._record[0] < second._record[0]);
}

struct DataRecordComparator {
    bool operator()(const DataRecord& first, const DataRecord& second) const {
        // Return true if first should go before second
		return first.is_smaller_str(second);
        // return true;
    }
};

void InternalSort(RecordList* records)
{
	string trace_str = "STATE -> SORT_MINI_RUNS: Sort cache-size mini runs";

	trace.append_trace(trace_str);

	records->record_ptr.sort(DataRecordComparator());
}
