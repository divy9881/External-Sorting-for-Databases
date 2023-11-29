#include "RecordList.h"

/**
RecordList* create_empty_record_list(int count_of_sorted_runs) {
    RecordList *new_list = (RecordList*) malloc(sizeof(RecordList)*count_of_sorted_runs);
    RecordList *iterator = new_list;
    for (int ii = 0 ; ii < count_of_sorted_runs; ii++) {
        iterator->record_count = 0;
        iterator->records = NULL;
        iterator++;
    }
    return new_list;
}

*/
/*
 * We use linked list, because we need to spill it to the persistent storage
 * and free up the memory
 * An array does not allow us to have any holes; so it cannot be freed up
 * unless whole array is merged.
 */
void append_to_record_list(RecordList* record_list, std::vector<DataRecord> *incoming_records) {
    for (long unsigned a = 0; a < incoming_records->size(); a++) {
        DataRecord temp = incoming_records->at(a);
        record_list->records.push_back(temp);
    }
    record_list->record_count += incoming_records->size();
}

DataRecord pop_record(RecordList *list) {
    DataRecord top = list->records[0];
    --list->record_count;
    list->records.erase(list->records.begin());
    return top;
}

DataRecord top_record(RecordList *list) {
    return list->records[0];
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

void InternalSort(RecordList* record_list)
{
    std::sort(record_list->records.begin(), record_list->records.end(), [](DataRecord &a, DataRecord &b){ return a._record[0] < b._record[0]; });
}
