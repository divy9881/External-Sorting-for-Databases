#include "StorageDevice.h"

StorageDevice::StorageDevice(string device_path, lluint total_space, uint max_runs)
{
	this->num_runs = 0;
	this->device_path = device_path;
	this->free_space = total_space;
	this->total_space = total_space;
	this->max_runs = max_runs;
	this->run_offsets = new lluint[max_runs];

	mkdir(this->device_path.c_str(), 0777);

	for(uint ii = 0 ; ii < this->max_runs ; ii++)
	{
		this->run_offsets[ii] = 0;
	}
}

void StorageDevice::spill_run(char run_bit, uint run, DataRecord *records, uint num_records)
{
	string run_path;
	if (run_bit == 'n') {
		run_path = this->device_path + "/run_" + to_string(this->num_runs);
		this->num_runs += 1;
	} else {
		run_path = this->device_path + "/run_" + to_string(run);
	}
	
	this->spill_run_to_disk(run_path, records, num_records);
	this->free_space -= num_records * ON_DISK_RECORD_SIZE;
}

pair<DataRecord *, uint> StorageDevice::get_run_page(uint run, uint num_records)
{
	pair<DataRecord *, uint> p;
	string run_path = this->device_path + "/run_" + to_string(run);

	p = this->get_run_page_from_disk(run_path, &this->run_offsets[run], num_records);

	return p;
}

void StorageDevice::truncate_device()
{
	truncate_all_runs();

	this->num_runs = 0;
	this->free_space = this->total_space;

	for(uint ii = 0 ; ii < this->max_runs ; ii++)
	{
		this->run_offsets[ii] = 0;
	}
}

void StorageDevice::spill_run_to_disk(string run_path, DataRecord *records, uint num_records)
{
	fstream runfile;
	string str_records;

	runfile.open(run_path, ios_base::app);
	if (!runfile.is_open())
		return;

	for (uint ii = 0 ; ii < num_records ; ii++) {
		DataRecord record = records[ii];
		string str_record = record.GetRecord();
		str_records += str_record + RECORD_DELIMITER;
	}
	runfile << str_records;

	runfile.close();
	return;
}

pair<DataRecord *, uint> StorageDevice::get_run_page_from_disk(string run_path, lluint *offset, uint num_records)
{
	fstream runfile;	
	DataRecord *records = new DataRecord[num_records];
	uint count_records = 0, ii = 0;
	pair<DataRecord *, uint> p;
	lluint col_value1, col_value2, col_value3, pos = 0;
	string record_delimiter(RECORD_DELIMITER);
	string column_delimiter(COLUMN_DELIMITER);
	string col_value;
	char *run_page = new char[num_records * ON_DISK_RECORD_SIZE + 1];

	runfile.open(run_path, ios::in);
	if (!runfile.is_open())
		return p;

	runfile.seekg(*offset, ios::beg);

	runfile.get(run_page, num_records * ON_DISK_RECORD_SIZE + 1);
	count_records = strlen(run_page) / ON_DISK_RECORD_SIZE;
	*offset += strlen(run_page);

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

		records[ii++].SetRecord(col_value1, col_value2, col_value3);
	}

	p.first = records;
	p.second = count_records;

	runfile.close();

	return p;
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