#include "Iterator.h"
#include "Scan.h"
#include "Filter.h"
#include "Sort.h"
#include "DataRecord.h"
#include "Tree.h"
#include "StorageDevice.h"
#include "SortRecords.h"
#include "defs.h"

// #define TEST_1 true
// #define TEST_2 true
// #define TEST_3 true
#define TEST_4 true
// #define TEST_5 true // Run Spilling on Disk and Reading Run Pages from Disk
// #define TEST_6 true // Internal sort on list of records
// #define TEST_7 true // Test get_last_run
// #define TEST_8 true // Test merging sorted runs on SSD
// #define TEST_9 true // Test merging sorted runs on HDD
// #define TEST_10 true // Test External Merge sort of 20 records

/*
 * Test configuration
 */
#define NUM_RECORDS 8
#define COUNT_OF_SORTED_RUNS 4

int main (int argc, char * argv [])
{
	TRACE (ENABLE_TRACE);
#if TEST_1
	Plan * const plan = new ScanPlan (7);
	// new SortPlan(new FilterPlan(newScanPlan(7)));
	Iterator * const it = plan->init ();
	DataRecord rec1 = DataRecord(1234, 1234, 1234);
	DataRecord rec2 = DataRecord(5678, 5678, 5678);
	DataRecord rec3 = DataRecord(9876, 9886, 9876);
	
	it->run ();

	delete it;
	delete plan;

	bool rec1_is_smaller_than_2 = rec1.is_smaller_int(rec2);
	if (rec1_is_smaller_than_2) {
		cout<<"Data record 1 is smaller than record 2"<<endl;
	} else {
		cout<<"Data record 1 is smaller than record 2"<<endl;
	}

	bool rec3_is_smaller_than_2 = rec3.is_smaller_int(rec2);
	if (rec3_is_smaller_than_2) {
		cout<<"Data record 3 is smaller than record 2"<<endl;
	} else {
		cout<<"Data record 3 is larger than record 2"<<endl;
	}

	rec2.populate_ovc(rec1);
	rec3.populate_ovc(rec2);

	rec2.print();
	rec3.print();

	DataRecord list1[NUM_RECORDS];

	for(int ii = 0; ii < NUM_RECORDS; ii++) {
		list1[ii].SetRecord(ii+1, ii+1, ii+1);
	}

	Tree test_tree = Tree(list1, 4, 1);
	test_tree.print_heap();
	test_tree.run_tree();
	test_tree.print_run();
#endif
#if TEST_2
	// Test 2, only to be used for merging - replace when a record list is empty
	cout<<"\n\n\n\t\t *********     TEST 2     *********"<<endl<<"\t\t\tTesting sorting of records\n\n\n";
	DataRecord list2[NUM_RECORDS];
	for(int ii = 0; ii < NUM_RECORDS; ii++) {
		list2[ii].SetRecord(ii+1, ii+1, ii+1);
	}

	// Tree *test_tree2 = new Tree(list2, 8, 1);
	// test_tree2->print_heap();
	// test_tree2->run_tree();
	// test_tree2->print_run();
	// test_tree2->print_heap();

	DataRecord list3[NUM_RECORDS];
	DataRecord list4[NUM_RECORDS];
	for(int ii = 0; ii < NUM_RECORDS; ii++) {
		list2[ii].SetRecord(ii+2, ii+2, ii+2);
		list3[ii].SetRecord(ii+1, ii+1, ii+1);
		list4[ii].SetRecord(ii+3, ii+3, ii+3);
	}

	Tree *test_tree3 = new Tree(list2, 8, 1);
	cout<<"The current heap is :: "<<endl; 
	test_tree3->print_heap();
	cout<<"Running the tree for "<< NUM_RECORDS <<" entries"<<endl;
	for (int ii = 0 ; ii < NUM_RECORDS ; ii++) {
		test_tree3->run_tree();
	}
	// test_tree3->print_heap();
	test_tree3->print_run();
	test_tree3->spillover_run();
#endif
#if TEST_3
	cout<<"\n\n\n\t\t*********     TEST 3     *********"<<endl<<"\tReplacement logic: Adding sorted runs at leaf nodes\n\n\n";
	// test_tree3->print_heap();
	cout<<"\nAdding new record list at leaves 7 and 8"<<endl;
	test_tree3->add_run_at_leaf(8, list3, 8);
	test_tree3->add_run_at_leaf(7, list4, 8);
	cout<<"\n\n\nAnd now, the heap is ::"<<endl;
	test_tree3->print_heap();
	cout<<"Running the tree now for 2 lists of size "<<NUM_RECORDS<<" ==> "<<(2*NUM_RECORDS)<<" iterations\n";
	for (int ii = 0 ; ii < NUM_RECORDS * 2 ; ii++) {
		test_tree3->run_tree();
	}
	cout<<"The generated run is: "<<endl;
	test_tree3->print_run();
#endif
#if TEST_4
	cout<<"\n\n\n\t\t*********     TEST 4     *********"<<endl<<"\tGenerate tree with sorted runs at leaf nodes, and dynamically add new records\n\n\n";
	DataRecord sorted_run[6][NUM_RECORDS];
	RecordList records[COUNT_OF_SORTED_RUNS];

	for (int jj = 0 ; jj < NUM_RECORDS ; jj++) {
		sorted_run[0][jj].SetRecord(jj+1, jj+1, jj+1);
		sorted_run[1][jj].SetRecord(jj+2, jj+2, jj+2);
		sorted_run[2][jj].SetRecord(jj+3, jj+3, jj+3);
		sorted_run[3][jj].SetRecord(jj+4, jj+4, jj+4);
		sorted_run[4][jj].SetRecord(jj+15, jj+5, jj+5);
		sorted_run[5][jj].SetRecord(jj+16, jj+6, jj+6);
	}
	vector<RecordList *> list_of_sorted_runs;

	for (uint jj = 0 ; jj < COUNT_OF_SORTED_RUNS ; jj++) {
		for (int ii = 0 ; ii < NUM_RECORDS; ii++) {
			records[jj].record_ptr.push_back(sorted_run[jj][ii]);
		}
		records[jj].record_count = NUM_RECORDS;
		list_of_sorted_runs.push_back(&records[jj]);
	}

	Tree test_tree4 = Tree(list_of_sorted_runs);
	cout<<"Printing the heap:"<<endl;
	test_tree4.print_heap();
	cout<<"Running the tree for "<<COUNT_OF_SORTED_RUNS * NUM_RECORDS<<"records\n";
	for (int ii = 0 ; ii < COUNT_OF_SORTED_RUNS * NUM_RECORDS ; ii++) {
		test_tree4.run_tree();
	}
	test_tree4.print_run();
	test_tree4.spillover_run();
	cout<<"\nAdding new sorted run at the position 3 and 6"<<endl;
	for (int ii = 0 ; ii < NUM_RECORDS ; ii++) {
		cout << " | sorted_run5 @ " << ii << ": ";
		sorted_run[4][ii].print();
		cout << " | sorted_run6 @ " << ii << ": ";
		sorted_run[5][ii].print();
		cout << endl;
	}
	test_tree4.add_run_at_leaf(3, sorted_run[4], NUM_RECORDS);
	test_tree4.add_run_at_leaf(6, sorted_run[5], NUM_RECORDS);
	test_tree4.print_heap();
	for (int ii = 0 ; ii < 2 * NUM_RECORDS; ii++) {
		test_tree4.run_tree();
	}
	test_tree4.print_run();
	test_tree4.spillover_run();
#endif
#if TEST_5
{
	StorageDevice ssd = StorageDevice("./SSD", (lluint)SSD_SIZE);
	vector<DataRecord> records;

	for (int ii = 0; ii < NUM_RECORDS; ii++)
	{
		DataRecord record = DataRecord(ii + 10, ii + 11, ii + 12);
		records.push_back(record);
	}

	ssd.spill_run('n', 0, records);

	records = ssd.get_run_page(0, NUM_RECORDS / 2);
	for (uint ii = 0; ii < records.size() ; ii++)
	{
		records[ii].print();
	}

	records = ssd.get_run_page(0, NUM_RECORDS / 2);
	for (uint ii = 0; ii < records.size(); ii++)
	{
		records[ii].print();
	}

	ssd.truncate_device();
}
#endif
#if TEST_6
{
	DataRecord *internal_sort_records = new DataRecord [NUM_RECORDS];
	RecordList record_list;

	cout << "Unsorted Data Records:" << endl;
	for (int ii = 0; ii < NUM_RECORDS; ii++)
	{
		internal_sort_records[ii].SetRecord(10 - ii, ii + 11, ii + 12);
		cout << internal_sort_records[ii].GetRecord() << endl;
	}

	record_list.record_ptr = internal_sort_records;
	record_list.record_count = NUM_RECORDS;

	InternalSort(&record_list);

	cout << "Sorted Data Records:" << endl;
	for (int ii = 0; ii < NUM_RECORDS; ii++)
	{
		cout << record_list.record_ptr[ii].GetRecord() << endl;
	}
	delete []internal_sort_records;
}
#endif
#if TEST_7
{
	StorageDevice ssd = StorageDevice("./SSD", (lluint)SSD_SIZE);
	vector<DataRecord> records;

	for (int ii = 0; ii < NUM_RECORDS; ii++)
	{
		DataRecord record = DataRecord(ii + 10, ii + 11, ii + 12);
		records.push_back(record);
	}

	ssd.spill_run('n', 0, records);
	cout << ssd.get_last_run() << endl;
	ssd.spill_run('n', 1, records);
	cout << ssd.get_last_run() << endl;
	ssd.truncate_device();
}
#endif
#if TEST_8
{
	StorageDevice ssd = StorageDevice("./SSD", (lluint)SSD_SIZE);
	StorageDevice hdd = StorageDevice("./HDD", (lluint)HDD_SIZE);
	vector<DataRecord> records;
	SortRecords sort = SortRecords(0, &ssd, &hdd);

	ssd.truncate_device();
	hdd.truncate_device();

	for (int ii = 0; ii < NUM_RECORDS; ii++)
	{
		DataRecord record = DataRecord(ii + 10, ii + 11, ii + 12);
		records.push_back(record);
	}
	ssd.spill_run('n', 0, records);
	cout << "Count of Records in SSD Device: " << ssd.get_num_records() << endl;

	records.clear();

	for (int ii = 0; ii < NUM_RECORDS; ii++)
	{
		DataRecord record = DataRecord(ii + 10, ii + 11, ii + 12);
		records.push_back(record);
	}
	ssd.spill_run('n', 0, records);
	cout << "Count of Records in SSD Device: " << ssd.get_num_records() << endl;

	sort.merge_runs_ssd();
	hdd.commit_temp_run();

	cout << "After merging sorted runs....." << endl;
	cout << "Count of Records in SSD Device: " << ssd.get_num_records() << endl;
	cout << "Count of Records in HDD Device: " << hdd.get_num_records() << endl;
}
#endif
#if TEST_9
{
	StorageDevice ssd = StorageDevice("./SSD", (lluint)SSD_SIZE);
	StorageDevice hdd = StorageDevice("./HDD", (lluint)HDD_SIZE);
	vector<DataRecord> records;
	SortRecords sort = SortRecords(0, &ssd, &hdd);

	ssd.truncate_device();
	hdd.truncate_device();

	for (int ii = 0; ii < NUM_RECORDS; ii++)
	{
		DataRecord record = DataRecord(ii + 10, ii + 11, ii + 12);
		records.push_back(record);
	}
	hdd.spill_run('n', 0, records);
	cout << "Count of Records in HDD Device: " << hdd.get_num_records() << endl;

	records.clear();

	for (int ii = 0; ii < NUM_RECORDS; ii++)
	{
		DataRecord record = DataRecord(ii + 10, ii + 11, ii + 12);
		records.push_back(record);
	}
	hdd.spill_run('n', 0, records);
	cout << "Count of Records in HDD Device: " << hdd.get_num_records() << endl;

	sort.merge_runs_hdd();

	cout << "After merging sorted runs....." << endl;
	cout << "Count of Records in SSD Device: " << ssd.get_num_records() << endl;
	cout << "Count of Records in HDD Device: " << hdd.get_num_records() << endl;
}
#endif
#if TEST_10
{
	StorageDevice ssd = StorageDevice("./SSD", (lluint)SSD_SIZE);
	StorageDevice hdd = StorageDevice("./HDD", (lluint)HDD_SIZE);
	SortRecords sort = SortRecords(20, &ssd, &hdd);

	ssd.truncate_device();
	hdd.truncate_device();

	cout << endl << "Sort 20 records..." << endl;
	sort.sort();

	// cout << "Count of Records in SSD Device: " << ssd.get_num_records() << endl;
	// cout << "Count of Records in HDD Device: " << hdd.get_num_records() << endl;
	cout << endl;
	cout << "Stats for SSD Device:" << endl;
	ssd.get_device_access_stats();
	cout << endl;
	cout << "Stats for HDD Device:" << endl;
	hdd.get_device_access_stats();

	sort = SortRecords(100, &ssd, &hdd);

	ssd.truncate_device();
	hdd.truncate_device();

	cout << endl << "Sort 100 records..." << endl;
	sort.sort();

	// cout << "Count of Records in SSD Device: " << ssd.get_num_records() << endl;
	// cout << "Count of Records in HDD Device: " << hdd.get_num_records() << endl;
	cout << endl;
	cout << "Stats for SSD Device:" << endl;
	ssd.get_device_access_stats();
	cout << endl;
	cout << "Stats for HDD Device:" << endl;
	hdd.get_device_access_stats();
/*
	sort = SortRecords(1000, &ssd, &hdd);

	ssd.truncate_device();
	hdd.truncate_device();

	cout << endl << "Sort 1000 records..." << endl;
	sort.sort();

	// cout << "Count of Records in SSD Device: " << ssd.get_num_records() << endl;
	// cout << "Count of Records in HDD Device: " << hdd.get_num_records() << endl;
	cout << endl;
	cout << "Stats for SSD Device:" << endl;
	ssd.get_device_access_stats();
	cout << endl;
	cout << "Stats for HDD Device:" << endl;
	hdd.get_device_access_stats();

	sort = SortRecords(10000, &ssd, &hdd);

	ssd.truncate_device();
	hdd.truncate_device();

	cout << endl << "Sort 2000 records..." << endl;
	sort.sort();

	// cout << "Count of Records in SSD Device: " << ssd.get_num_records() << endl;
	// cout << "Count of Records in HDD Device: " << hdd.get_num_records() << endl;
	cout <<endl;
	cout << "Stats for SSD Device:" << endl;
	ssd.get_device_access_stats();
	cout << endl;
	cout << "Stats for HDD Device:" << endl;
	hdd.get_device_access_stats();

	sort = SortRecords(10000, &ssd, &hdd);

	ssd.truncate_device();
	hdd.truncate_device();

	cout << endl << "Sort 10000 records..." << endl;
	sort.sort();

	cout << "Count of Records in SSD Device: " << ssd.get_num_records() << endl;
	cout << "Count of Records in HDD Device: " << hdd.get_num_records() << endl;
	cout << endl;
	cout << "Stats for SSD Device:" << endl;
	ssd.get_device_access_stats();
	cout << endl;
	cout << "Stats for HDD Device:" << endl;
	hdd.get_device_access_stats();

	sort = SortRecords(100000, &ssd, &hdd);

	ssd.truncate_device();
	hdd.truncate_device();

	cout << endl << "Sort 100000 records..." << endl;
	sort.sort();

	// cout << "Count of Records in SSD Device: " << ssd.get_num_records() << endl;
	// cout << "Count of Records in HDD Device: " << hdd.get_num_records() << endl;
	cout << endl;
	cout << "Stats for SSD Device:" << endl;
	ssd.get_device_access_stats();
	cout << endl;
	cout << "Stats for HDD Device:" << endl;
	hdd.get_device_access_stats();

	sort = SortRecords(1200000, &ssd, &hdd);

	ssd.truncate_device();
	hdd.truncate_device();

	cout << endl << "Sort 1200000 records..." << endl;
	sort.sort();

	// cout << "Count of Records in SSD Device: " << ssd.get_num_records() << endl;
	// cout << "Count of Records in HDD Device: " << hdd.get_num_records() << endl;
	cout << endl;
	cout << "Stats for SSD Device:" << endl;
	ssd.get_device_access_stats();
	cout << endl;
	cout << "Stats for HDD Device:" << endl;
	hdd.get_device_access_stats();
*/
}
#endif
return 0;
} // main
