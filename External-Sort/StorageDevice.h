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
		int get_num_records();
		lluint get_free_space();
		vector<DataRecord> get_run_page(uint, uint);
		pair<vector<RecordList *>, lluint> get_run_pages(uint);
		void spill_run(char, uint, vector<DataRecord>);
        bool verify_sort_result(lluint input_record_count, lluint output_record_count);
        void spill_runs(vector<RecordList *> record_lists);
        void commit_temp_run();
		void truncate_device();
		void get_device_access_stats();

	protected:
		string device_path;
		lluint free_space;
		lluint total_space;
		lluint *run_offsets;
		lluint total_reads;
		lluint total_writes;

		lluint get_run_num_records(uint run);
		void spill_run_to_disk(string, vector<DataRecord>);
		vector<DataRecord> get_run_page_from_disk(string, lluint *, uint);
		int truncate_all_runs();
};