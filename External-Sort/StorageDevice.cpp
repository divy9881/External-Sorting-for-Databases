#include "StorageDevice.h"

StorageDevice::StorageDevice(string device_path, lluint total_space)
{
	this->device_path = device_path;
	this->free_space = total_space;
	this->total_space = total_space;
	this->run_offsets = new lluint[MAX_RUNS];
	this->total_reads = 0;
	this->total_writes = 0;

	mkdir(this->device_path.c_str(), 0777);

	for(uint ii = 0 ; ii < MAX_RUNS ; ii++)
	{
		this->run_offsets[ii] = 0;
	}
}

int StorageDevice::get_last_run()
{
	int n;
	uint run;
	struct dirent **namelist;

	n = scandir(this->device_path.c_str(), &namelist, 0, alphasort);

	if (n <= 2) return -1;
	else sscanf((char *)&namelist[n - 1]->d_name[4], "%u", &run);

	return run;
}

uint StorageDevice::get_num_runs()
{
	int n;
	struct dirent **namelist;

	n = scandir(this->device_path.c_str(), &namelist, 0, alphasort);

	return n - 2;
}

void StorageDevice::spill_run(char run_bit, uint run, vector<DataRecord> records)
{
	string run_path;
	if (run_bit == 'n') {
		int last_run = this->get_last_run();
		run_path = this->device_path + "/run_" + to_string(last_run + 1);
	} else {
		run_path = this->device_path + "/run_" + to_string(run);
	}
	
	this->spill_run_to_disk(run_path, records);
	this->free_space -= records.size() * ON_DISK_RECORD_SIZE;

	this->total_writes += 1;
}

vector<DataRecord> StorageDevice::get_run_page(uint run, uint num_records)
{
	vector<DataRecord> records;
	string run_path = this->device_path + "/run_" + to_string(run);

	records = this->get_run_page_from_disk(run_path, &this->run_offsets[run], num_records);

	this->total_reads += 1;

	return records;
}

vector<RecordList *> StorageDevice::get_run_pages(uint num_records)
{
	uint n;
	struct dirent **namelist;
	vector<RecordList *> record_lists;

	n = scandir(this->device_path.c_str(), &namelist, 0, alphasort);

	for (uint ii = 2 ; ii < n ; ii++) {
		uint run;
		vector<DataRecord> records;
		string run_path;
		DataRecord *record_objs;
		RecordList *list = new RecordList;

		sscanf((char *)&namelist[ii]->d_name[4], "%u", &run);

		records = this->get_run_page(run, num_records);

		record_objs = new DataRecord[records.size()];
		for (uint jj = 0 ; jj < records.size() ; jj++) {
			record_objs[jj] = records[jj];
			record_objs[jj].print();
		}

		list->record_ptr = record_objs;
		list->record_count = records.size();

		record_lists.push_back(list);
	}

	return record_lists;
}

void StorageDevice::truncate_device()
{
	truncate_all_runs();

	this->free_space = this->total_space;

	for(uint ii = 0 ; ii < MAX_RUNS ; ii++)
	{
		this->run_offsets[ii] = 0;
	}
}

void StorageDevice::spill_run_to_disk(string run_path, vector<DataRecord> records)
{
	fstream runfile;
	string str_records;

	runfile.open(run_path, ios_base::app);
	if (!runfile.is_open())
		return;

	for (uint ii = 0 ; ii < records.size() ; ii++) {
		DataRecord record = records[ii];
		string str_record = record.GetRecord();
		str_records += str_record + RECORD_DELIMITER;
	}
	runfile << str_records;

	runfile.close();
	return;
}

vector<DataRecord> StorageDevice::get_run_page_from_disk(string run_path, lluint *offset, uint num_records)
{
	fstream runfile;	
	vector<DataRecord> records;
	lluint col_value1, col_value2, col_value3, pos = 0;
	string record_delimiter(RECORD_DELIMITER);
	string column_delimiter(COLUMN_DELIMITER);
	string col_value;
	char *run_page = new char[num_records * ON_DISK_RECORD_SIZE + 1];

	runfile.open(run_path, ios::in);
	if (!runfile.is_open())
		return records;

	runfile.seekg(*offset, ios::beg);

	runfile.get(run_page, num_records * ON_DISK_RECORD_SIZE + 1);
	runfile.close();
	*offset += strlen(run_page);

	/*
	 * Remove file if we have exhausted all the records
	 * from the run file
	 */
	if (strlen(run_page) != (num_records * ON_DISK_RECORD_SIZE)) {
		remove(run_path.c_str());
	}

	string s(run_page);
	/*
	 * Iterating over records
	 */
	while ((pos = s.find(record_delimiter)) != string::npos)
	{
		stringstream ss1, ss2, ss3;
		string token;

		token = s.substr(0, pos);
		s.erase(0, pos + record_delimiter.length());
		
		/* 
		 * For Column Value 1
		 */
		pos = token.find(column_delimiter);
		col_value = token.substr(0, pos);
		ss1 << col_value;
		ss1 >> col_value1;
		token.erase(0, pos + column_delimiter.length());
		/*
		 * For Column Value 2
		 */
		pos = token.find(column_delimiter);
		col_value = token.substr(0, pos);
		ss2 << col_value;
		ss2 >> col_value2;
		token.erase(0, pos + column_delimiter.length());
		/*
		 * For Column Value 3
		 */
		col_value = token;
		ss3 << col_value;
		ss3 >> col_value3;
		DataRecord record = DataRecord(col_value1, col_value2, col_value3);
		records.push_back(record);
	}

	delete[] run_page;

	return records;
}

int StorageDevice::truncate_all_runs()
{
	struct stat sb;
	string device_path = this->device_path;
	DIR *device = opendir(device_path.c_str());
	struct dirent *next_run;
	char runpath[257];

	if (stat(device_path.c_str(), &sb))
		return 0;

	while ((next_run = readdir(device)) != NULL)
	{
		sprintf(runpath, "%s/%s", device_path.c_str(), next_run->d_name);
		remove(runpath);
	}
	closedir(device);

	return rmdir(device_path.c_str());
}