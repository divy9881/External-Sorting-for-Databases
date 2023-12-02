#include "Iterator.h"
#include "Scan.h"
#include "Filter.h"
#include "Sort.h"
#include "DataRecord.h"
#include "Tree.h"
#include "StorageDevice.h"
#include "SortRecords.h"
#include "defs.h"

int main(int argc, char *argv[])
{
    char *str_num_records, *str_record_size, *str_trace_file;
    char trace_file[256 + 1] = "trace";
    llint c;
    lluint num_records = 20, record_size = 6, col_value_length = NUM_CHARS_COL_VALUE;

    while ((c = getopt(argc, argv, "c:s:o:")) != (llint)-1)
    {
        switch (c)
        {
            case 'c':
                str_num_records = optarg;
                num_records = atoi(str_num_records);
                break;
            case 's':
                str_record_size = optarg;
                record_size = atoi(str_record_size);
                break;
            case 'o':
                str_trace_file = optarg;
                strcpy(trace_file, str_trace_file);
                break;
            default:
                abort();
        }
    }

    col_value_length = record_size / 3;

    StorageDevice ssd = StorageDevice("./SSD", (lluint)SSD_SIZE, col_value_length);
    StorageDevice hdd = StorageDevice("./HDD", (lluint)HDD_SIZE, col_value_length);
    SortRecords sort = SortRecords(num_records, &ssd, &hdd, col_value_length);

    ssd.truncate_device();
    hdd.truncate_device();

    cout << endl << "Sort "<< num_records << " records..." << endl;
    sort.sort();

    cout << endl;
    cout << "Stats for SSD Device:" << endl;
    ssd.get_device_access_stats();
    cout << endl;
    cout << "Stats for HDD Device:" << endl;
    hdd.get_device_access_stats();

    return 0;
}