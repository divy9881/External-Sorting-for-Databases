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

pair<DataRecord *, uint> StorageDevice::get_run_page(uint run)
{
    pair<DataRecord *, uint> p;
    string run_path = this->device_path + "/run_" + to_string(this->num_runs);

    p = this->get_run_page_from_disk(run_path, &this->run_offsets[run], 1);

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
    ofstream runfile(run_path);

    for (uint ii = 0 ; ii < num_records ; ii++) {
        DataRecord record = records[ii];

        runfile.write((char *)&record, sizeof(record));
        runfile << '\n';
    }
    runfile.close();
	return;
}

pair<DataRecord *, uint> StorageDevice::get_run_page_from_disk(string run_path, uint *offset, uint num_records)
{
    ifstream runfile(run_path);
	DataRecord *records = new DataRecord[num_records];
    uint count_records = 0;
    pair<DataRecord *, uint> p;
    char line[1];

    while(count_records < *offset) {
        runfile.getline(line, 1);
        count_records += 1;
    }

    count_records = 0;
    for (uint ii = 0 ; ii < num_records && !runfile.eof() ; ii++) {
        runfile.read((char *)&records[ii], sizeof(records[ii]));
        runfile >> line;
        count_records += 1;
        *offset += 1;
    }

    p.first = records;
    p.second = count_records;

	return p;
}

int StorageDevice::truncate_all_runs()
{
    struct stat sb;
    string device_path = this->device_path;

    if (!stat(device_path.c_str(), &sb))
        return 0;
    
    return rmdir(device_path.c_str());
}