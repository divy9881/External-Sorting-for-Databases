#include "Iterator.h"
#include "Scan.h"
#include "Filter.h"
#include "Sort.h"
#include "DataRecord.h"
#include "Tree.h"
#include "StorageDevice.h"
#include "SortRecords.h"
#include "defs.h"
#include "SortTrace.h"

// #define TEST_1 true
// #define TEST_2 true
// #define TEST_3 false
// #define TEST_4 false
// #define TEST_5 false // Run Spilling on Disk and Reading Run Pages from Disk
// #define TEST_6 false // Internal sort on list of records
// #define TEST_7 false // Test get_last_run
// #define TEST_8 false // Test merging sorted runs on SSD
// #define TEST_9 true // Test merging sorted runs on HDD
#define TEST_10 true // Test External Merge sort of 20 records

#define CLEAN_RECORDS(records) 	for (lluint iter = 0 ; iter < records.size(); iter++) {\
		delete records[iter];\
	}

char trace_file[256 + 1] = "trace";
SortTrace trace(trace_file);

/*
 * Test configuration
 */
#define NUM_RECORDS 8
#define COUNT_OF_SORTED_RUNS 4

int main (int argc, char * argv [])
{
	TRACE (ENABLE_TRACE);
#if TEST_1
	Plan * const plan = new ScanPlan (7, 2);
	// new SortPlan(new FilterPlan(newScanPlan(7)));
	Iterator * const it = plan->init ();
	DataRecord rec1 = DataRecord("1234", "1234","1234", 4);
	DataRecord rec2 = DataRecord("5678", "5678","5678", 4);
	DataRecord rec3 = DataRecord("9876", "9886","9876", 4);
	
	it->run ();

	delete it;
	delete plan;

	bool rec1_is_smaller_than_2 = rec1.is_smaller_str(rec2);
	if (rec1_is_smaller_than_2) {
		cout<<"Data record 1 is smaller than record 2"<<endl;
	} else {
		cout<<"Data record 1 is smaller than record 2"<<endl;
	}

	bool rec3_is_smaller_than_2 = rec3.is_smaller_str(rec2);
	if (rec3_is_smaller_than_2) {
		cout<<"Data record 3 is smaller than record 2"<<endl;
	} else {
		cout<<"Data record 3 is larger than record 2"<<endl;
	}

	rec2.populate_ovc_str(rec1);
	rec3.populate_ovc_str(rec2);

	rec2.print();
	rec3.print();

	DataRecord list1[NUM_RECORDS];

	for(int ii = 0; ii < NUM_RECORDS; ii++) {
		list1[ii].SetRecord(to_string(ii+1), to_string(ii+1), to_string(ii+1), 1);
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
		list2[ii].SetRecord(to_string(ii)+'1', to_string(ii)+'1', to_string(ii)+'1', 1);
	}

	// Tree *test_tree2 = new Tree(list2, 8, 1);
	// test_tree2->print_heap();
	// test_tree2->run_tree();
	// test_tree2->print_run();
	// test_tree2->print_heap();

	DataRecord list3[NUM_RECORDS];
	DataRecord list4[NUM_RECORDS];
	for(int ii = 0; ii < NUM_RECORDS; ii++) {
		list2[ii].SetRecord(to_string(ii)+'2', to_string(ii)+'2', to_string(ii)+'2', 1);
		list3[ii].SetRecord(to_string(ii)+'1', to_string(ii)+'1', to_string(ii)+'1', 1);
		list4[ii].SetRecord(to_string(ii)+'3', to_string(ii)+'3', to_string(ii)+'3', 1);
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
		for (int ii = 0 ; ii < 6 ; ii++) {
			string rec1 = pick(DB_COL_SIZE); string rec2 = pick(DB_COL_SIZE); string rec3 = pick(DB_COL_SIZE);
			sorted_run[ii][jj].SetRecord(rec1, rec2, rec3, DB_COL_SIZE);
		}
		// sorted_run[0][jj].SetRecord("abcd123", "abcd123", "abcd123", DB_COL_SIZE);
		// sorted_run[1][jj].SetRecord("bcd2345", "bcd2345", "bcd2345", DB_COL_SIZE);
		// sorted_run[2][jj].SetRecord("cde3456", "cde3456", "cde3456", DB_COL_SIZE);
		// sorted_run[3][jj].SetRecord("def4567", "def4567", "def4567", DB_COL_SIZE);
		// sorted_run[4][jj].SetRecord("efg5678", "efg5678", "efg5678", DB_COL_SIZE);
		// sorted_run[5][jj].SetRecord("fgh6789", "fgh6789", "fgh6789", DB_COL_SIZE);
		// sorted_run[0][jj].SetRecord(pick(DB_COL_SIZE), pick(DB_COL_SIZE), pick(DB_COL_SIZE), DB_COL_SIZE);
		// sorted_run[1][jj].SetRecord(pick(DB_COL_SIZE), pick(DB_COL_SIZE), pick(DB_COL_SIZE), DB_COL_SIZE);
		// sorted_run[2][jj].SetRecord(pick(DB_COL_SIZE), pick(DB_COL_SIZE), pick(DB_COL_SIZE), DB_COL_SIZE);
		// sorted_run[3][jj].SetRecord(pick(DB_COL_SIZE), pick(DB_COL_SIZE), pick(DB_COL_SIZE), DB_COL_SIZE);
		// sorted_run[4][jj].SetRecord(pick(DB_COL_SIZE), pick(DB_COL_SIZE), pick(DB_COL_SIZE), DB_COL_SIZE);
		// sorted_run[5][jj].SetRecord(pick(DB_COL_SIZE), pick(DB_COL_SIZE), pick(DB_COL_SIZE), DB_COL_SIZE);
	}
	vector<RecordList *> list_of_sorted_runs;

	for (uint jj = 0 ; jj < COUNT_OF_SORTED_RUNS ; jj++) {
		for (int ii = 0 ; ii < NUM_RECORDS; ii++) {
			records[jj].record_ptr.push_back(sorted_run[jj][ii]);
		}
		records[jj].record_count = NUM_RECORDS;
		InternalSort(&records[jj]);
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
	// RecordList *temp = new RecordList;
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
	StorageDevice ssd = StorageDevice("./SSD", (lluint)SSD_SIZE, 2);
	vector <DataRecord *> records_to_spill;
	vector <DataRecord> run_read;

	for (int ii = 0; ii < NUM_RECORDS; ii++)
	{
		string s1 = pick(DB_COL_SIZE); string s2 = pick(DB_COL_SIZE); string s3 = pick(DB_COL_SIZE);
		DataRecord *record = new DataRecord(s1, s2, s3, DB_COL_SIZE);
		records_to_spill.push_back(record);
		record->print();
	}

	ssd.spill_run('n', 0, records_to_spill);
	cout << "Record spill DONE"<<endl<<flush;
	run_read = ssd.get_run_page(0, NUM_RECORDS / 2);
	cout << "Run pages READ from the file 1 - DONE"<<endl<<flush;
	for (uint ii = 0; ii < run_read.size() ; ii++)
	{
		run_read[ii].print();
	}
	cout << "Run pages READ from the file 2 - DONE"<<endl<<flush;
	run_read = ssd.get_run_page(0, NUM_RECORDS / 2);
	for (uint ii = 0; ii < run_read.size(); ii++)
	{
		run_read[ii].print();
	}

	ssd.truncate_device();
	cout << "Device truncate - DONE"<<endl<<flush;
	CLEAN_RECORDS(records_to_spill);
}
#endif
#if TEST_6
{
	list<DataRecord> internal_sort_records;
	RecordList record_list;

	cout << "Unsorted Data Records:" << endl;
	for (int ii = 0; ii < NUM_RECORDS; ii++)
	{
		string s1 = pick(DB_COL_SIZE); string s2 = pick(DB_COL_SIZE); string s3 = pick(DB_COL_SIZE);
		DataRecord newRec(s1, s2, s3, DB_COL_SIZE);
		internal_sort_records.push_back(newRec);
		printf("%.10s\n", newRec._record[0]);
		// cout << newRec._record[0] << endl;
	}

	record_list.record_ptr = internal_sort_records;
	record_list.record_count = NUM_RECORDS;

	InternalSort(&record_list);

	cout << "Sorted Data Records:" << endl;
	for (auto iter: record_list.record_ptr)
	{
		// cout << iter._record[0] << endl<<flush;
		printf("%.10s\n", iter._record[0]);
	}
}
#endif
#if TEST_7
{
	StorageDevice ssd = StorageDevice("./SSD", (lluint)SSD_SIZE, 2);
	vector <DataRecord *> records;

	for (int ii = 0; ii < NUM_RECORDS; ii++)
	{
		DataRecord *record = new DataRecord(to_string(ii + 10), to_string(ii + 11), to_string(ii + 12), 2);
		records.push_back(record);
	}

	ssd.spill_run('n', 0, records);
	cout << ssd.get_last_run() << endl;
	ssd.spill_run('n', 1, records);
	cout << ssd.get_last_run() << endl;
	ssd.truncate_device();
	CLEAN_RECORDS(records);

}
#endif
#if TEST_8
{
	StorageDevice ssd = StorageDevice("./SSD", (lluint)SSD_SIZE, 2);
	StorageDevice hdd = StorageDevice("./HDD", (lluint)HDD_SIZE, 2);
	vector <DataRecord *> records;
	SortRecords sort = SortRecords(0, &ssd, &hdd, 2);

	ssd.truncate_device();
	hdd.truncate_device();

	for (int ii = 0; ii < NUM_RECORDS; ii++)
	{
		string s1 = pick(DB_COL_SIZE), s2 = pick(DB_COL_SIZE), s3 = pick(DB_COL_SIZE);
		DataRecord* record = new DataRecord(s1, s2, s3, DB_COL_SIZE);
		records.push_back(record);
	}
	ssd.spill_run('n', 0, records);
	cout << "Count of Records in SSD Device: " << ssd.get_num_records() << endl;

	CLEAN_RECORDS(records);
	records.clear();

	for (int ii = 0; ii < NUM_RECORDS; ii++)
	{
		string s1 = pick(DB_COL_SIZE), s2 = pick(DB_COL_SIZE), s3 = pick(DB_COL_SIZE);
		DataRecord* record = new DataRecord(s1, s2, s3, DB_COL_SIZE);

		records.push_back(record);
	}
	ssd.spill_run('n', 0, records);
	cout << "Count of Records in SSD Device: " << ssd.get_num_records() << endl;
	CLEAN_RECORDS(records)

	sort.merge_runs_ssd();
	hdd.commit_temp_run();

	cout << "After merging sorted runs....." << endl;
	cout << "Count of Records in SSD Device: " << ssd.get_num_records() << endl;
	cout << "Count of Records in HDD Device: " << hdd.get_num_records() << endl;
}
#endif
#if TEST_9
{
	StorageDevice ssd = StorageDevice("./SSD", (lluint)SSD_SIZE, 2);
	StorageDevice hdd = StorageDevice("./HDD", (lluint)HDD_SIZE, 2);
	vector <DataRecord *> records;
	SortRecords sort = SortRecords(0, &ssd, &hdd, 2);

	ssd.truncate_device();
	hdd.truncate_device();

	for (int ii = 0; ii < NUM_RECORDS; ii++)
	{
		DataRecord *record = new DataRecord(to_string(ii + 10), to_string(ii + 11), to_string(ii + 12), 2);
		records.push_back(record);
	}
	hdd.spill_run('n', 0, records);
	cout << "Count of Records in HDD Device: " << hdd.get_num_records() << endl;

	CLEAN_RECORDS(records);
	records.clear();

	for (int ii = 0; ii < NUM_RECORDS; ii++)
	{
		DataRecord *record = new DataRecord(to_string(ii + 10), to_string(ii + 11), to_string(ii + 12), 2);
		records.push_back(record);
	}
	hdd.spill_run('n', 0, records);
	cout << "Count of Records in HDD Device: " << hdd.get_num_records() << endl;

	sort.merge_runs_hdd();
	CLEAN_RECORDS(records);

	cout << "After merging sorted runs....." << endl;
	cout << "Count of Records in SSD Device: " << ssd.get_num_records() << endl;
	cout << "Count of Records in HDD Device: " << hdd.get_num_records() << endl;
}
#endif
#if TEST_10
{
	StorageDevice ssd = StorageDevice("./SSD", (lluint)SSD_SIZE, DB_COL_SIZE);
	StorageDevice hdd = StorageDevice("./HDD", (lluint)HDD_SIZE, DB_COL_SIZE);
	SortRecords sort = SortRecords(20, &ssd, &hdd, DB_COL_SIZE);

	ssd.truncate_device();
	hdd.truncate_device();

	cout << endl << "Sort 20 records..." << endl;
	sort.sort();

	cout << endl;
	cout << "Stats for SSD Device:" << endl;
	ssd.get_device_access_stats();
	cout << endl;
	cout << "Stats for HDD Device:" << endl;
	hdd.get_device_access_stats();

	sort = SortRecords(100, &ssd, &hdd, DB_COL_SIZE);

	ssd.truncate_device();
	hdd.truncate_device();

	cout << endl << "Sort 100 records..." << endl;
	sort.sort();

	cout << endl;
	cout << "Stats for SSD Device:" << endl;
	ssd.get_device_access_stats();
	cout << endl;
	cout << "Stats for HDD Device:" << endl;
	hdd.get_device_access_stats();

	sort = SortRecords(1000, &ssd, &hdd, DB_COL_SIZE);

	ssd.truncate_device();
	hdd.truncate_device();

	cout << endl << "Sort 1000 records..." << endl;
	sort.sort();

	cout << endl;
	cout << "Stats for SSD Device:" << endl;
	ssd.get_device_access_stats();
	cout << endl;
	cout << "Stats for HDD Device:" << endl;
	hdd.get_device_access_stats();

	sort = SortRecords(2000, &ssd, &hdd, DB_COL_SIZE);

	ssd.truncate_device();
	hdd.truncate_device();

	cout << endl << "Sort 2000 records..." << endl;
	sort.sort();

	cout <<endl;
	cout << "Stats for SSD Device:" << endl;
	ssd.get_device_access_stats();
	cout << endl;
	cout << "Stats for HDD Device:" << endl;
	hdd.get_device_access_stats();

	sort = SortRecords(10000, &ssd, &hdd, DB_COL_SIZE);

	ssd.truncate_device();
	hdd.truncate_device();

	cout << endl << "Sort 10000 records..." << endl;
	sort.sort();

	cout << endl;
	cout << "Stats for SSD Device:" << endl;
	ssd.get_device_access_stats();
	cout << endl;
	cout << "Stats for HDD Device:" << endl;
	hdd.get_device_access_stats();

	sort = SortRecords(100000, &ssd, &hdd, DB_COL_SIZE);

	ssd.truncate_device();
	hdd.truncate_device();

	cout << endl << "Sort 100000 records..." << endl;
	sort.sort();

	cout << endl;
	cout << "Stats for SSD Device:" << endl;
	ssd.get_device_access_stats();
	cout << endl;
	cout << "Stats for HDD Device:" << endl;
	hdd.get_device_access_stats();

	sort = SortRecords(1200000, &ssd, &hdd, DB_COL_SIZE);

	ssd.truncate_device();
	hdd.truncate_device();

	cout << endl << "Sort 1200000 records..." << endl;
	sort.sort();

	cout << endl;
	cout << "Stats for SSD Device:" << endl;
	ssd.get_device_access_stats();
	cout << endl;
	cout << "Stats for HDD Device:" << endl;
	hdd.get_device_access_stats();

}
#endif
return 0;
} // main
