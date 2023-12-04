#include "Table.h"

Table::Table(char table_file[256 + 1], uint col_value_length)
{
	this->num_records = 0;
	this->col_value_length = col_value_length;
	strcpy(this->table_file, table_file);
	this->truncate_table();
	mkdir(this->table_file, 0777);
}

void Table::persist_records(RecordList *records)
{
	fstream tablefile;
	string str_records = "";
	string table_file = string(this->table_file);
	table_file += "/table";

	tablefile.open(table_file, ios_base::app);
	if (!tablefile.is_open())
		return;

	this->num_records += records->record_count;

	for (auto a: records->record_ptr) {
		DataRecord record(a);
		string str_record = record.GetRecord();
		str_records += str_record + TABLE_RECORD_DELIMITER;
	}

	tablefile << str_records;

	tablefile.close();

	return;
}

void Table::persist_records_from_run(StorageDevice *device)
{
	uint page_num_records = OPTIMAL_HDD_PAGE_SIZE / ON_DISK_RECORD_SIZE(this->col_value_length);

	while (device->get_num_runs())
	{
		pair<vector<RecordList *>, lluint> p;

		p = device->get_run_pages(page_num_records);

		this->persist_records(p.first[0]);
	}

	return;
}

void Table::truncate_table()
{
	struct stat sb;
	DIR *table = opendir(this->table_file);
	struct dirent *next_table;

	if (stat(this->table_file, &sb))
		return;

	while ((next_table = readdir(table)) != NULL)
	{
		string tablepath = string(this->table_file);
		string tablename = string(next_table->d_name);
		tablepath += "/" + tablename;
		remove(tablepath.c_str());
	}
	closedir(table);

	return;
}