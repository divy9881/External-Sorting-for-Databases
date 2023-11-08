#pragma once

#include "defs.h"
#include "databaseConfig.h"
#include "DataRecord.h"

class StorageDevice
{
    public:
        StorageDevice(string, lluint, uint);
        DataRecord* get_run_page(uint);
        void spill_run(DataRecord *, uint);
        void truncate_device();
    protected:
        uint num_runs;
        uint max_runs;
        string device_path;
        lluint free_space;
        lluint total_space;
        uint *run_offsets;

        void spill_run_to_disk(string, DataRecord *, uint);
        DataRecord * get_run_page_from_disk(string, uint *);
        void truncate_all_runs(string);
};