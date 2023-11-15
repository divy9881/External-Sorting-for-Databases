#include "StorageDevice.h"

StorageDevice::StorageDevice(string device_path, lluint total_space, uint max_runs)
{
	this->num_runs = 0;
	this->device_path = device_path;
	this->free_space = total_space;
	this->total_space = total_space;
	this->max_runs = max_runs;
	this->run_offsets = new uint[max_runs];

	mkdir(this->device_path.c_str(), 0777);

	for(uint ii = 0 ; ii < this->max_runs ; ii++)
	{
		this->run_offsets[ii] = 0;
	}
}

void StorageDevice::spill_run(DataRecord *records, uint num_records)
{
	string run_path = this->device_path + "/run_" + to_string(this->num_runs);

	this->spill_run_to_disk(run_path, records, num_records);
	this->free_space = num_records * ON_DISK_RECORD_SIZE;
	this->num_runs += 1;
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

	runfile.open(run_path, ios::out);
	if (!runfile.is_open())
		return;

	for (uint ii = 0 ; ii < num_records ; ii++) {
		DataRecord record = records[ii];
		string str_record = record.GetRecord();
		runfile << str_record << endl;
	}

	runfile.close();
	return;
}

pair<DataRecord *, uint> StorageDevice::get_run_page_from_disk(string run_path, uint *offset, uint num_records)
{
	fstream runfile;	
	DataRecord *records = new DataRecord[num_records];
	uint count_records = 0;
	pair<DataRecord *, uint> p;
	lluint record_value, record_value1, record_value2, record_value3;

	runfile.open(run_path, ios::in);
	if (!runfile.is_open())
		return p;

	while(count_records < *offset) {
		runfile >> record_value >> record_value >> record_value;
		count_records += 1;
	}

	count_records = 0;
	for (uint ii = 0 ; ii < num_records && !runfile.eof() ; ii++) {
		runfile >> record_value1;
		runfile >> record_value2;
		runfile >> record_value3;
		records[ii].SetRecord(record_value1, record_value2, record_value3);
		count_records += 1;
		*offset += 1;
	}

	p.first = records;
	p.second = count_records;

	runfile.close();

	return p;
}

/*
 * TODO: removing all the runs and device directory not working
 */
int StorageDevice::truncate_all_runs()
{
	struct stat sb;
	string device_path = this->device_path;

	if (!stat(device_path.c_str(), &sb))
		return 0;
	
	return rmdir(device_path.c_str());
}