#include "SortRecords.h"

SortRecords::SortRecords(lluint num_records, StorageDevice *ssd_device, StorageDevice *hdd_device, uint col_value_length)
{
	this->num_records = num_records;
	this->ssd_device = ssd_device;
	this->hdd_device = hdd_device;
	this->col_value_length = col_value_length;
}

void SortRecords::sort()
{
	lluint rem_num_records = this->num_records;
	uint run_size = DRAM_SIZE / OPTIMAL_FAN_IN;
	lluint num_records_per_run = run_size / ON_DISK_RECORD_SIZE(this->col_value_length);
	char input_table_file[256 + 1] = "input";
	char output_table_file[256 + 1] = "output";
	Table input_table = Table(input_table_file, this->col_value_length);
	Table output_table = Table(output_table_file, this->col_value_length);

	while (rem_num_records)
	{
		uint rem_dram_size_num_records = min(DRAM_SIZE / ON_DISK_RECORD_SIZE(this->col_value_length), rem_num_records);
		lluint count_of_records = 0;
		vector<RecordList *> runs;
		while (rem_dram_size_num_records)
		{
			lluint gen_num_records = 0;
			if (num_records_per_run >= rem_dram_size_num_records)
			{
				rem_num_records -= rem_dram_size_num_records;
				gen_num_records = rem_dram_size_num_records;
				rem_dram_size_num_records = 0;
			}
			else
			{
				rem_dram_size_num_records -= num_records_per_run;
				rem_num_records -= num_records_per_run;
				gen_num_records = num_records_per_run;
			}
			ScanPlan p = ScanPlan(gen_num_records, this->col_value_length);
			/*
			 * Get records list from the ScanPlan object
			 * Make sure you free RecordList struct and the underlyiing
			 * DataRecord's using delete operator
			 */
			RecordList *records = p.GetRecords();
			input_table.persist_records(records);
			count_of_records += records->record_count;
			InternalSort(records);
			runs.push_back(records);
		}

		if (count_of_records * ON_DISK_RECORD_SIZE(this->col_value_length) <= this->ssd_device->get_free_space())
		{
			this->ssd_device->spill_runs(runs);
		}
		else
		{
			this->hdd_device->spill_runs(runs);
		}
	}
	this->merge_runs_ssd();
	this->hdd_device->commit_temp_run();
	this->merge_runs_hdd();

	output_table.persist_records_from_run(this->hdd_device);
}

void SortRecords::merge_runs_ssd()
{
	uint ssd_page_num_records = OPTIMAL_SSD_PAGE_SIZE / ON_DISK_RECORD_SIZE(this->col_value_length);

	while (this->ssd_device->get_num_runs()) {
		lluint num_records;
		vector<DataRecord> records;
		vector<RecordList *> record_lists;
		pair <vector<RecordList *>, lluint> p;

		p = this->ssd_device->get_run_pages(ssd_page_num_records);
		record_lists = p.first;
		num_records = p.second;

		if (record_lists.size() > 1) {
			Tree tree = Tree(record_lists);

			for (uint ii = 0; ii < num_records; ii++)
			{
				tree.run_tree();
			}

			records = tree.get_generated_run();
			// TODO: fix freeing of memory
			//       currently there is a memory link
			//       but the below statement corrupt the memory
			//       for large number of records
			// for (uint ii = 0; ii < record_lists.size(); ii++)
			// {
			// 	delete[] record_lists[ii]->record_ptr;
			// 	delete record_lists[ii];
			// }
		} else {
			for (uint ii = 0; ii < record_lists[0]->record_count; ii++)
			{
				records.push_back(record_lists[0]->record_ptr[ii]);
			}

			delete [] record_lists[0]->record_ptr;
			delete record_lists[0];
		}
		this->hdd_device->spill_run('t', -1, records);
	}

	return;
}

void SortRecords::merge_runs_hdd()
{
	uint hdd_page_num_records = OPTIMAL_HDD_PAGE_SIZE / ON_DISK_RECORD_SIZE(this->col_value_length);

	while (this->hdd_device->get_num_runs())
	{
		vector<DataRecord> records;
		vector<RecordList *> record_lists;
		pair <vector<RecordList *>, lluint> p;

		p = this->hdd_device->get_run_pages(hdd_page_num_records);
		record_lists = p.first;

		this->ssd_device->spill_runs(record_lists);
		this->merge_runs_ssd();
	}

	this->hdd_device->commit_temp_run();

	return;
}