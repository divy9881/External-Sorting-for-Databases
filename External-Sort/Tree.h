#pragma once

#include "defs.h"
#include "DataRecord.h"

/*
 * @struct RecordList
 * Wrapper for the (list of) data records and the count of records in the list
 * Has two associated functions, pop_record and top_record
 */
typedef struct RecordList {
    DataRecord* data = NULL;
    lluint record_count = 0;
} RecordList;


/*
 * @struct Node
 * is_empty -> Indicates the node does not have any data
 * is_leaf -> Indicates that the node is a leaf node
 * current_record -> Used only for the internal nodes, stores the loser node at this position
 * List -> List of all records in the _leaf_ node (only)
 * level -> indicates the level of the node in the tree
 */
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
    lluint total_leaves, total_nodes, tree_depth;
    lluint total_record_count;
public:
    Tree();
    Tree(int);
    Tree(DataRecord *, int, int);
    Node* getRoot();
    void run_tree();
    int capacity(int level);
    struct Node leaf(int index, int slot);
    struct Node parent(int slot);
    void compare_and_swap(int, int);
    vector<int> get_empty_leaves();
    int add_records_at_leaf(int, DataRecord *, int);
    void print_heap();
    void print_run();
    ~Tree();
};
