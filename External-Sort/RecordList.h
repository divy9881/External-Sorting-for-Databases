#pragma once

#ifndef RECORDLIST_H
#define RECORDLIST_H

#include "DataRecord.h"

/*
 * @struct RecordList
 * Wrapper for the (list of) data records and the count of records in the list
 * Has two associated functions, pop_record and top_record
 */
typedef struct RecordList {
    std::vector <DataRecord> records;
    lluint record_count = 0;
} RecordList;

// RecordList* create_empty_record_list(int count_of_sorted_runs);
void append_to_record_list(RecordList* record_list, std::vector<DataRecord> *incoming_records);
DataRecord top_record(RecordList *list);
DataRecord pop_record(RecordList *list);

void InternalSort(RecordList* records);
int comparator(const void *arg1, const void *arg2);

#endif //RECORDLIST_H
