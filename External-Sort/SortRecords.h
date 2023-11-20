#pragma once

#include "Scan.h"
#include "sortParams.h"
#include "StorageDevice.h"
#include "Tree.h"

class SortRecords {
	public:
		SortRecords(lluint num_records, StorageDevice *, StorageDevice *);
		void sort();
		void merge_runs_ssd();
		void merge_runs_hdd();
	private:
		lluint num_records;
		StorageDevice *ssd_device;
		StorageDevice *hdd_device;
};

