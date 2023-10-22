#include "Tree.h"

#include <cmath>

#define HEAP_RECORD_LIST_AT(ii) this->heap[ii].list.data
#define NODE_RECORD_EMPTY(node) node->is_empty
#define IS_LEAF_NODE(node) node->is_leaf
#define NODE_CURRENT_RECORD(node) node->current_record
#define HEAP_RECORD_EMPTY(ii) this->heap[ii].is_empty
#define NODE_RECORD_LIST(node) node->list->data
#define NODE_RECORD_LIST_AT(node, idx) node->list->data[idx]
#define NODE_RECORD_LIST_LENGTH(node) node->list->record_count


DataRecord* pop_record(RecordList *list) {
    DataRecord* top = NULL;
    if (list->record_count == 1) {
        top = &(list->data[0]);
        list->data = NULL;
    } else if (list->record_count > 1) {
        top = &(list->data[0]);
        list->data = &(list->data[1]);
    }
    list->record_count--;
    return top;
}

DataRecord* top_record(RecordList *list) {
    return list->data;
}


Tree::Tree(DataRecord **records, int record_ct)
{
    this->generateTournamentTree(records, record_ct);
}

int Tree::capacity(int level) {
    return (1<<level);
}

void Tree::generateTournamentTree(DataRecord **all_records, int record_ct)
{
    // TODO See if this is optimal division for fanning
    this->total_leaves = ceil(log2(record_ct));
    int count_of_cols_per_row = ceil(record_ct/this->total_leaves)+1;

    printf("Total Leaves are %d, per row there are %d columns", this->total_leaves, count_of_cols_per_row);

    this->tree_depth = ceil(log2(this->total_leaves));
    this->total_nodes = 2 * pow(2, this->tree_depth) - 1;
    this->heap = std::vector <struct Node>(this->total_nodes);

    printf("Size of the heap is %ld\n", this->heap.size());
    int first_leaf_node = this->total_nodes - ((this->total_nodes - 1)/2) - 1;
    printf("first_leaf_node starts as %d\n", first_leaf_node);
    int record_iterator = 0, current_ct = record_ct;
    printf("Total count would be %d\n", first_leaf_node + this->total_leaves);

    for (int ii = first_leaf_node ; ii < (this->total_leaves*2) - 1 ; ii++) {
        if (record_iterator >= record_ct) {
            this->heap[ii].is_empty = true;
            this->heap[ii].is_leaf = true;
            continue;
        }
        // Leaf nodes has no current record
        this->heap[ii].current_record = NULL;
        this->heap[ii].is_empty = false;
        this->heap[ii].is_leaf = true;

        // Assign records to each row
        printf("allocated\n");
        printf("Assigning %d per row\n", count_of_cols_per_row);
        this->heap[ii].list = (RecordList*) malloc(sizeof(RecordList));
        this->heap[ii].list->data = (DataRecord*) malloc(sizeof(DataRecord*));
        this->heap[ii].list->data = (*all_records) + record_iterator;
        this->heap[ii].list->record_count = count_of_cols_per_row;

        record_iterator += count_of_cols_per_row;
        // Sample calculation:
        // For 128 records, there will be 7 rows == 18 count_of_cols_per_row + 2 remaining
        // for last row, current_ct will be 2 after subtraction, so we will just add it to the last row
        current_ct -= count_of_cols_per_row;
        printf("Current remaining: %d\n",current_ct);
        if (current_ct <= 0) {
            continue;
        } else if (current_ct < count_of_cols_per_row) {
            count_of_cols_per_row += current_ct;
        }
    }
}

void Tree::compare_and_swap(int parent, int unused_leaves_idx) {
    int child_left = parent*2+1, child_right=parent*2*2;
    DataRecord* winner_record = NULL;
    struct Node *parent_node = &this->heap[parent];
    // None of the children are valid == not being used as runs
    if (child_left >= unused_leaves_idx) {
        return;
    } else if (child_left < unused_leaves_idx /*Only left child is valid, special case only valid at leaf nodes */ 
                && child_right >= unused_leaves_idx) {
            struct Node *left_child_node = &this->heap[child_left];
            // If parent has a valid record, it will be the winner, so skip.
            // Iteration over the next level will empty this node
            // the iteration after that for this level would fill it up.
            if (parent_node->current_record) {
                return;
            } else {
                if (IS_LEAF_NODE(left_child_node)) {
                    parent_node->current_record = pop_record(left_child_node->list);
                }
            }
    } else { /* Both the children are valid. So add the popped record at parent (if empty) */
        if (parent_node->current_record) {
            return;
        } else {
            struct Node *left_child_node = &this->heap[child_left];
            struct Node *right_child_node = &this->heap[child_right];
            DataRecord *left_data = NULL, *right_data = NULL;
            if (IS_LEAF_NODE(left_child_node)) {
                // Both will be a leaf node
                DataRecord *left_data = top_record(left_child_node->list);
                DataRecord *right_data = top_record(right_child_node->list);
                if (left_data && right_data) {
                    // Compare
                    if (left_data->_record[0] <= right_data->_record[0]) {
                        parent_node->current_record = pop_record(left_child_node->list);
                    } else {
                        parent_node->current_record = pop_record(right_child_node->list);
                    }
                } else if (left_data) {
                    parent_node->current_record = pop_record(left_child_node->list);
                } else if (right_data) {
                    parent_node->current_record = pop_record(right_child_node->list);
                } else {
                    parent_node->current_record = NULL;
                }
            } else {
                // It is an internal node
                DataRecord *left_data = NODE_CURRENT_RECORD(left_child_node);
                DataRecord *right_data = NODE_CURRENT_RECORD(right_child_node);
                if (left_data && right_data) {
                    // Compare
                    if (left_data->_record[0] <= right_data->_record[0]) {
                        parent_node->current_record = left_data;
                        NODE_CURRENT_RECORD(left_child_node) = NULL;
                    } else {
                        parent_node->current_record = right_data;
                        NODE_CURRENT_RECORD(right_child_node) = NULL;
                    }
                } else if (left_data) {
                    parent_node->current_record = left_data;
                    NODE_CURRENT_RECORD(left_child_node) = NULL;
                } else if (right_data) {
                    parent_node->current_record = right_data;
                    NODE_CURRENT_RECORD(right_child_node) = NULL;
                } else {
                    parent_node->current_record = NULL;
                }
            }
        }
    }
}

struct Node Tree::leaf(int index, int current_slot) {
    return this->heap[current_slot*2 + index];
}

struct Node Tree::parent(int current_slot) {
    return this->heap[current_slot/2];
}

void Tree::run_tree() {
    int unused_leaves_idx = (this->total_nodes + 1) / 2 - 1 + this->total_leaves;
    // At the end of the loop, the 0th position will have the smallest element
    // For each inner node (parents), check the leaves level-wise
    int current_level = this->total_leaves - 1;
    for (int iteration = 0 ; iteration < this->total_record_count; iteration++) {
        // Each iteration will give one of the priority queue elements
        for (int inner_node_index = pow(2, this->tree_depth) - 2; inner_node_index >= 0; inner_node_index--) {
            this->compare_and_swap(inner_node_index, unused_leaves_idx);
        }
        this->generated_run.push_back(*(this->heap[0].current_record));
    }
    for (auto a: generated_run) {
        a.print();
    }
}

void Tree::print_heap() {
    for (int ii = 0; ii < this->heap.size(); ii++) {
        if (1) {
            if (this->heap[ii].current_record) {
                // printf("The data is %d\n", this->heap[ii].data[0]._record[0]);
                printf("null");
            } 
        }
    }
}

Tree::~Tree ()
{
	TRACE (true);
	// delete root;
}
