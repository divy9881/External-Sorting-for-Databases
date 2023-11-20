#include "SortRecords.h"

SortRecords::SortRecords(lluint num_records, StorageDevice *ssd_device, StorageDevice *hdd_device)
{
	this->num_records = num_records;
	this->ssd_device = ssd_device;
	this->hdd_device = hdd_device;
}

void SortRecords::sort()
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

void SortRecords::merge_runs_ssd()
{
	uint last_run_hdd = this->hdd_device->get_last_run();
	uint ssd_page_num_records = OPTIMAL_SSD_PAGE_SIZE / ON_DISK_RECORD_SIZE;

	while (this->ssd_device->get_num_runs()) {
		vector<RecordList *> record_lists;
		vector<DataRecord> records;
		record_lists = this->ssd_device->get_run_pages(ssd_page_num_records);

		Tree tree = Tree(record_lists);
		cout << "Print tree" << endl;
		tree.print_heap();
		for (uint ii = 0; ii < record_lists.size(); ii++)
		{
			for (uint jj = 0 ; jj < record_lists[ii]->record_count ; jj++)
			{
				tree.run_tree();
			}
		}
		cout << "Print run" << endl;
		tree.print_run();
		records = tree.get_generated_run();

		this->hdd_device->spill_run('o', last_run_hdd + 1, records);
	}

	return;
}

void SortRecords::merge_runs_hdd()
{

}