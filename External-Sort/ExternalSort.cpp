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
    llint c;
    lluint num_records, record_size;
    //Iterator iterator;
    StorageDevice ssd = StorageDevice("./SSD", (lluint)SSD_SIZE);
    StorageDevice hdd = StorageDevice("./HDD", (lluint)HDD_SIZE);

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
                break;
            default:
                abort();
        }
    }

    SortRecords sort = SortRecords(num_records, &ssd, &hdd);

    ssd.truncate_device();
    hdd.truncate_device();

    cout << endl << "Sort "<< num_records << " records..." << endl;
    sort.sort();

    int ssd_record_number = ssd.get_num_records();
    int hdd_record_number = hdd.get_num_records();

    //Verification of the records
    if (num_records != hdd_record_number){
        cout<< "Error: Mismatch number of HDD records and input records" << endl;
    }else{
        cout << "Number of HDD Records: " << hdd_record_number << endl;
        cout << "Number of input Records: " << num_records << endl;
    }
    //verification of sort order
    Iterator::verifySortOrder("HDD");


    cout << "Count of Records in SSD Device: " << ssd_record_number << endl;
    cout << "Count of Records in HDD Device: " << hdd_record_number << endl;

    cout << endl;
    cout << "Stats for SSD Device:" << endl;
    ssd.get_device_access_stats();
    cout << endl;
    cout << "Stats for HDD Device:" << endl;
    hdd.get_device_access_stats();

    return 0;
}