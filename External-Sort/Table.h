#pragma once

#include "defs.h"
#include "databaseConfig.h"
#include "DataRecord.h"
#include "StorageDevice.h"

class Table {
	public:
		Table(char table_file[256 + 1], uint);
		void persist_records(RecordList *records);
		void persist_records_from_run(StorageDevice *device);
		void truncate_table();
	private:
		lluint num_records;
		uint col_value_length;
		char table_file[256 + 1];
};