#include "SortRecords.h"

SortRecords::SortRecords(lluint num_records)
{
	this->num_records = num_records;
}

void SortRecords::run()
{
	lluint rem_num_records = this->num_records;
	uint run_size = DRAM_SIZE / OPTIMAL_FAN_IN;
	lluint num_records_per_run = run_size / ON_DISK_RECORD_SIZE;

	while (rem_num_records)
	{
		uint rem_dram_size_num_records = min(DRAM_SIZE / ON_DISK_RECORD_SIZE, rem_num_records);
		vector<RecordList *> runs;
		while (rem_dram_size_num_records)
		{
			lluint gen_num_records = 0;
			if (num_records_per_run >= rem_dram_size_num_records)
			{
				rem_dram_size_num_records = 0;
				rem_num_records -= rem_dram_size_num_records;
				gen_num_records = rem_dram_size_num_records;
			}
			else
			{
				rem_dram_size_num_records -= num_records_per_run;
				rem_num_records -= num_records_per_run;
				gen_num_records = num_records_per_run;
			}
			ScanPlan p = ScanPlan(gen_num_records);
			/*
			* Get records list from the ScanPlan object
			* Make sure you free RecordList struct and the underlyiing
			* DataRecord's using delete operator
			*/
			RecordList *records = p.GetRecords();
			InternalSort(records);
			runs.push_back(records);
		}
	}
}
