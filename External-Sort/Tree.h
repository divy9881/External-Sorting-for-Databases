#pragma once

#include <iostream>
#include <vector>
#include <limits>
#include "DataRecord.h"

using namespace std;


typedef struct RecordList {
    DataRecord* data = NULL;
    long record_count = 0;
} RecordList;

struct Node {
    // Current record is used only for the inner nodes
    DataRecord *current_record;
    // List of all data records
    // data is used only for the leaf nodes
    RecordList *list;
    bool is_empty;
    bool is_leaf;
    int level;
    Node() {
        current_record = NULL;
        is_empty = true;
        is_leaf = false;
        level = 0;
    }
    // Node(const DataRecord& val) : data(val), left((Node*)nullptr), right((Node*)nullptr) {}
};

class Tree
{
private:
    std::vector <struct Node> heap;
    std::vector <struct DataRecord*> generated_run;
    int total_leaves, total_nodes, tree_depth;
    unsigned long long total_record_count;
public:
    Tree();
    Tree(DataRecord *, int);
    Node* getRoot();
    void run_tree(DataRecord **);
    void generateTournamentTree(DataRecord *, int);
    // DataRecord getWinner();
    void run_tree();
    int capacity(int level);
    struct Node leaf(int index, int slot);
    struct Node parent(int slot);
    void compare_and_swap(int, int);
    void print_heap();
    void print_run();
    // void getSortedDataRecordsCaller();
    // void getSortedDataRecords(Node* root, std::set<DataRecord>& result);
    ~Tree();
};
