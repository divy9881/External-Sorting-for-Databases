#include "Iterator.h"
#include "Scan.h"
#include "Filter.h"
#include "Sort.h"
#include "DataRecord.h"
#include "Tree.h"

#define TEST_1 false
#define TEST_2 true
#define TEST_3 false
#define TEST_4 true

int main (int argc, char * argv [])
{
	// TRACE (true);
	int num_records = 8;
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
		std::cout<<"Data record 3 is smaller than record 2"<<endl;
	} else {
		std::cout<<"Data record 3 is larger than record 2"<<endl;
	}

	rec2.populate_ovc(rec1);
	rec3.populate_ovc(rec2);

	rec2.print();
	rec3.print();

	DataRecord *list1 = (DataRecord*)malloc(sizeof(DataRecord) * num_records);

	for(int ii = 0; ii < num_records; ii++) {
		list1[ii].SetRecord(ii+1, ii+1, ii+1);
	}

	Tree *test_tree = new Tree(list1, 4, 1);
	test_tree->print_heap();
	test_tree->run_tree();
	test_tree->print_run();
#endif
#if TEST_2
	// Test 2, only to be used for merging - replace when a record list is empty
	cout<<"\n\n\n\t\t *********     TEST 2     *********"<<endl<<"\t\t\tTesting sorting of records\n\n\n";
	DataRecord *list2 = (DataRecord*)malloc(sizeof(DataRecord) * num_records);
	for(int ii = 0; ii < num_records; ii++) {
		list2[ii].SetRecord(ii+1, ii+1, ii+1);
	}

	// Tree *test_tree2 = new Tree(list2, 8, 1);
	// test_tree2->print_heap();
	// test_tree2->run_tree();
	// test_tree2->print_run();
	// test_tree2->print_heap();

	// num_records = 8;
	DataRecord *list3 = (DataRecord*)malloc(sizeof(DataRecord) * num_records);
	DataRecord *list4 = (DataRecord*)malloc(sizeof(DataRecord) * num_records);
	for(int ii = 0; ii < num_records; ii++) {
		list2[ii].SetRecord(ii+2, ii+2, ii+2);
		list3[ii].SetRecord(ii+1, ii+1, ii+1);
		list4[ii].SetRecord(ii+3, ii+3, ii+3);
	}

	Tree *test_tree3 = new Tree(list2, 8, 1);
	cout<<"The current heap is :: "<<endl; 
	test_tree3->print_heap();
	cout<<"Running the tree for "<< num_records<<" entries"<<endl;
	for (int ii = 0; ii < num_records; ii++) {
		test_tree3->run_tree();
	}
	// test_tree3->print_heap();
	test_tree3->print_run();
	test_tree3->spillover_run();
#endif
#if TEST_3
	cout<<"\n\n\n\t\t*********     TEST 3     *********"<<endl<<"\tReplacement logic: Adding sorted runs at leaf nodes\n\n\n";
	// test_tree3->print_heap();
	num_records = 8;
	cout<<"\nAdding new record list at leaves 7 and 8"<<endl;
	test_tree3->add_run_at_leaf(8, list3, 8);
	test_tree3->add_run_at_leaf(7, list4, 8);
	cout<<"\n\n\nAnd now, the heap is ::"<<endl;
	test_tree3->print_heap();
	cout<<"Running the tree now for 2 lists of size "<<num_records<<" ==> "<<(2*num_records)<<" iterations\n";
	for (int ii = 0 ; ii < num_records*2; ii++) {
		test_tree3->run_tree();
	}
	cout<<"The generated run is: "<<endl;
	test_tree3->print_run();
#endif
#if TEST_4
	cout<<"\n\n\n\t\t*********     TEST 4     *********"<<endl<<"\tGenerate tree with sorted runs at leaf nodes, and dynamically add new records\n\n\n";
	int count_of_sorted_runs = 3;
	num_records = 8;
	DataRecord *sorted_run1 = (DataRecord*)malloc(sizeof(DataRecord) * num_records);
	DataRecord *sorted_run2 = (DataRecord*)malloc(sizeof(DataRecord) * num_records);
	DataRecord *sorted_run3 = (DataRecord*)malloc(sizeof(DataRecord) * num_records);
	DataRecord *sorted_run4 = (DataRecord*)malloc(sizeof(DataRecord) * num_records);
	DataRecord *sorted_run5 = (DataRecord*)malloc(sizeof(DataRecord) * num_records);
	DataRecord *sorted_run6 = (DataRecord*)malloc(sizeof(DataRecord) * num_records);

	for (int jj = 0 ; jj < num_records ; jj++) {
		sorted_run1[jj].SetRecord(jj+1, jj+1, jj+1);
		sorted_run2[jj].SetRecord(jj+2, jj+2, jj+2);
		sorted_run3[jj].SetRecord(jj+3, jj+3, jj+3);
		sorted_run4[jj].SetRecord(jj+4, jj+4, jj+4);
		sorted_run5[jj].SetRecord(jj+5, jj+5, jj+5);
		sorted_run6[jj].SetRecord(jj+6, jj+6, jj+6);
	}
	RecordList *list_of_sorted_runs = (RecordList*) malloc(sizeof(RecordList)*count_of_sorted_runs);
	list_of_sorted_runs[0].record_ptr = sorted_run1; list_of_sorted_runs[0].record_count = num_records;
	list_of_sorted_runs[1].record_ptr = sorted_run2; list_of_sorted_runs[1].record_count = num_records;
	list_of_sorted_runs[2].record_ptr = sorted_run3; list_of_sorted_runs[2].record_count = num_records;
	// list_of_sorted_runs[3].record_ptr = sorted_run4; list_of_sorted_runs[3].record_count = num_records;

	Tree *test_tree4 = new Tree(list_of_sorted_runs, count_of_sorted_runs);
	cout<<"Printing the heap:"<<endl;
	test_tree4->print_heap();
	cout<<"Running the tree for "<<count_of_sorted_runs*num_records<<"records\n";
	for (int ii = 0 ; ii < count_of_sorted_runs*num_records ; ii++) {
		test_tree4->run_tree();
	}
	test_tree4->print_run();
	test_tree4->spillover_run();
	cout<<"\nAdding new sorted run at the position 3 and 6"<<endl;
	for (int ii = 0 ; ii < num_records; ii++) {
		cout<<" | sorted_run5 @ "<<ii<<": ";sorted_run5[ii].print();
		cout<<" | sorted_run6 @ "<<ii<<": "; sorted_run6[ii].print();
		cout<<endl;
	}
	test_tree4->add_run_at_leaf(3, sorted_run5, num_records);
	test_tree4->add_run_at_leaf(6, sorted_run6, num_records);
	test_tree4->print_heap();
	for (int ii = 0 ; ii < 2*num_records; ii++) {
		test_tree4->run_tree();
	}
	test_tree4->print_run();
	test_tree4->spillover_run();
#endif
	return 0;
} // main
