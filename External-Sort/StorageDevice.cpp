#include "StorageDevice.h"

StorageDevice::StorageDevice(string device_path, lluint total_space, uint max_runs)
{
    this->num_runs = 0;
    this->device_path = device_path;
    this->free_space = total_space;
    this->total_space = total_space;
    this->max_runs = max_runs;
    this->run_offsets = new uint[max_runs];

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

DataRecord * StorageDevice::get_run_page(uint run)
{
    DataRecord *records;
    string run_path = this->device_path + "/run_" + to_string(this->num_runs);

    records = this->get_run_page_from_disk(run_path, &this->run_offsets[run]);
    // this->run_offsets[run] = offset;

    return records;
}

void StorageDevice::truncate_device()
{
    truncate_all_runs(this->device_path);

    this->num_runs = 0;
    this->free_space = this->total_space;

    for(uint ii = 0 ; ii < this->max_runs ; ii++)
    {
        this->run_offsets[ii] = 0;
    }
}

void StorageDevice::spill_run_to_disk(string run_path, DataRecord *records, uint num_records)
{
	return;
}

DataRecord * StorageDevice::get_run_page_from_disk(string run_path, uint *offset)
{
	DataRecord *records = new DataRecord[1];

	return records;
}

void StorageDevice::truncate_all_runs(string device_path)
{
    return;
}