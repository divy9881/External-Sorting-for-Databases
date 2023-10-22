// g++ defs.cpp  Test_tree.cpp Tree.cpp DataRecord.cpp -o Test_tree.exe

#include "DataRecord.h"
#include "Tree.h"
#include <iostream>
#include <vector>

using namespace std;

int main() {
    DataRecord **list1 = (DataRecord**)malloc(sizeof(DataRecord) * 3);
    for (int ii =0; ii<3; ii++) {
        list1[ii] = (DataRecord*)malloc(sizeof(DataRecord) * 3);
    } 
    DataRecord rec1 = DataRecord(1234, 1234, 1234);
    DataRecord rec2 = DataRecord(2345, 2345, 2345);
    DataRecord rec3 = DataRecord(3456, 3456, 3456);
    DataRecord rec4 = DataRecord(5678, 5678, 5678);
    DataRecord rec5 = DataRecord(9876, 9886, 9876);
    DataRecord rec6 = DataRecord(6789, 6789, 6789);
    DataRecord rec7 = DataRecord(2524, 2152, 6348);
    DataRecord rec8 = DataRecord(2379, 2532, 6343);
    DataRecord rec9 = DataRecord(5923, 4732, 2358);

    list1[0][0] = rec1;
    list1[0][1] = rec2;
    list1[0][2] = rec3;

    list1[1][0] = rec4;
    list1[1][1] = rec5;
    list1[1][2] = rec6;

    list1[2][0] = rec7;
    list1[2][1] = rec8;
    list1[2][2] = rec9;
    
    Tree *test_tree = new Tree(list1,9);
    // test_tree.print_heap();
    // test_tree.getWinner().print();
    return 0;
}