#pragma once

#include "defs.h"
#include "databaseConfig.h"
#include "DataRecord.h"

class StorageDevice
{
	public:
		StorageDevice(string, lluint);
		int get_last_run();
		uint get_num_runs();
		vector<DataRecord> get_run_page(uint, uint);
		vector<RecordList *> get_run_pages(uint);
		void spill_run(char, uint, vector<DataRecord>);
		void spill_runs(vector<RecordList *> record_lists);
		void truncate_device();

	protected:
		string device_path;
		lluint free_space;
		lluint total_space;
		lluint *run_offsets;
		lluint total_reads;
		lluint total_writes;

		void spill_run_to_disk(string, vector<DataRecord>);
		vector<DataRecord> get_run_page_from_disk(string, lluint *, uint);
		int truncate_all_runs();
};