#pragma once

#include "defs.h"
#include "databaseConfig.h"
#include "DataRecord.h"

class StorageDevice
{
	public:
		StorageDevice(string, lluint, uint);
		vector<DataRecord> get_run_page(uint, uint);
		void spill_run(char, uint, vector<DataRecord>);
		void truncate_device();
	protected:
		uint num_runs;
		uint max_runs;
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