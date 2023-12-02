#pragma once

#include "Scan.h"
#include "sortParams.h"
#include "StorageDevice.h"
#include "Tree.h"
#include "Table.h"

class SortRecords {
	public:
		SortRecords(lluint num_records, StorageDevice *, StorageDevice *, uint);
		void sort();
		void merge_runs_ssd();
		void merge_runs_hdd();
	private:
		lluint num_records;
		StorageDevice *ssd_device;
		StorageDevice *hdd_device;
		uint col_value_length;
};

