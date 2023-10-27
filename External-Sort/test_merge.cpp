// g++ defs.cpp  Test_tree.cpp Tree.cpp DataRecord.cpp -o Test_tree.exe

#include "DataRecord.h"
#include "Merge.h"
#include <iostream>
#include <vector>

using namespace std;

int main() {
    DataRecord *list1 = (DataRecord*)malloc(sizeof(DataRecord) * 9);
    // for (int ii =0; ii<3; ii++) {
    //     list1[ii] = (DataRecord*)malloc(sizeof(DataRecord) * 3);
    // }

    DataRecord rec1 = DataRecord(1, 1, 1);
    DataRecord rec2 = DataRecord(200, 2, 2);
    DataRecord rec3 = DataRecord(3, 3, 3);
    DataRecord rec4 = DataRecord(-4, 4, 4);
    DataRecord rec5 = DataRecord(5, 5, 5);
    DataRecord rec6 = DataRecord(65, 6, 6);
    DataRecord rec7 = DataRecord(27, 7, 7);
    DataRecord rec8 = DataRecord(8, 8, 8);
    DataRecord rec9 = DataRecord(9999, 9, 9);

    list1[0] = rec1;
    list1[1] = rec2;
    list1[2] = rec3;

    list1[3] = rec4;
    list1[4] = rec5;
    list1[5] = rec6;

    list1[6] = rec7;
    list1[7] = rec8;
    list1[8] = rec9;
    
    Merge *test_ = new Merge(list1, 9);
    test_->printDataRecords();
    
    return 0;
}