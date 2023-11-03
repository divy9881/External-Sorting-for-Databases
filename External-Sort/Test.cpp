#include "Iterator.h"
#include "Scan.h"
#include "Filter.h"
#include "Sort.h"
#include "DataRecord.h"
#include "Tree.h"

int main (int argc, char * argv [])
{
	// TRACE (true);

	// Plan * const plan = new ScanPlan (7);
	// // new SortPlan(new FilterPlan(newScanPlan(7)));
	// Iterator * const it = plan->init ();
	// DataRecord rec1 = DataRecord(1234, 1234, 1234);
	// DataRecord rec2 = DataRecord(5678, 5678, 5678);
	// DataRecord rec3 = DataRecord(9876, 9886, 9876);
	int num_records = 4;
	
	// it->run ();

	// delete it;
	// delete plan;

	// bool rec1_is_smaller_than_2 = rec1.is_smaller_int(rec2);
	// if (rec1_is_smaller_than_2) {
	// 	cout<<"Data record 1 is smaller than record 2"<<endl;
	// } else {
	// 	cout<<"Data record 1 is smaller than record 2"<<endl;
	// }

	// bool rec3_is_smaller_than_2 = rec3.is_smaller_int(rec2);
	// if (rec3_is_smaller_than_2) {
	// 	std::cout<<"Data record 3 is smaller than record 2"<<endl;
	// } else {
	// 	std::cout<<"Data record 3 is larger than record 2"<<endl;
	// }

	// rec2.populate_ovc(rec1);
	// rec3.populate_ovc(rec2);

	// rec2.print();
	// rec3.print();

	// DataRecord *list1 = (DataRecord*)malloc(sizeof(DataRecord) * num_records);

	// for(int ii = 0; ii < num_records; ii++) {
	// 	list1[ii].SetRecord(ii+1, ii+1, ii+1);
	// }

	// Tree *test_tree = new Tree(list1, 4, 1);
	// test_tree->print_heap();
	// test_tree->run_tree();
	// test_tree->print_run();

	// Test 2, only to be used for merging - replace when a record list is empty
	num_records = 8;
	DataRecord *list2 = (DataRecord*)malloc(sizeof(DataRecord) * num_records);
	for(int ii = 0; ii < num_records; ii++) {
		list2[ii].SetRecord(ii+1, ii+1, ii+1);
	}

	Tree *test_tree2 = new Tree(list2, 8, 1);
	test_tree2->print_heap();
	test_tree2->run_tree();
	test_tree2->print_run();
	test_tree2->print_heap();

	return 0;
} // main
