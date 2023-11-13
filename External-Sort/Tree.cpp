#include "Tree.h"

#define HEAP_RECORD_LIST_AT(ii) this->heap[ii].list.record_ptr
#define NODE_RECORD_EMPTY(node) node->is_empty
#define IS_LEAF_NODE(node) node->is_leaf
#define NODE_CURRENT_RECORD(node) node->current_record
#define HEAP_RECORD_EMPTY(ii) this->heap[ii].is_empty
#define NODE_RECORD_LIST(node) node->list->record_ptr
#define NODE_RECORD_LIST_AT(node, idx) node->list->record_ptr[idx]
#define NODE_RECORD_LIST_LENGTH(node) node->list->record_count
#define CHECK_SET_EMPTY(node, node_idx)\
            if (node->list->record_count == 0) {\
                node->is_empty = true;\
            }
#define LAST_DATARECORD(temp, record_list)\
            temp = record_list->record_ptr;\
            while(temp->next != NULL)\
                {temp = temp->next;}

#define ADD_RECORDS(last_record, new_record_list, count)\
            for (lluint ii = 0 ; ii < count; ii++) {\
                last_record->next = (DataRecord*) malloc(sizeof(DataRecord));\
                last_record->next->ov_code.ovc = 0;\
                last_record->next->ov_code.rel = '\0'; \
                last_record->next->_record[0] = last_record->next->_record[1] = last_record->next->_record[2] = 0;\
                (last_record->next)->SetRecord(new_record_list[ii]._record[0], new_record_list[ii]._record[1], new_record_list[ii]._record[2]);\
                last_record = last_record->next;\
                last_record->next=NULL;\
            }



RecordList* create_empty_record_list(int count_of_sorted_runs) {
    RecordList *new_list = (RecordList*) malloc(sizeof(RecordList)*count_of_sorted_runs);
    RecordList *iterator = new_list;
    for (int ii = 0 ; ii < count_of_sorted_runs; ii++) {
        iterator->record_count = 0;
        iterator->record_ptr = NULL;
        iterator++;
    }
    return new_list;
} 

/*
 * We use linked list, because we need to spill it to the persistent storage
 * and free up the memory
 * An array does not allow us to have any holes; so it cannot be freed up
 * unless whole array is merged.
 */
void append_to_record_list(RecordList** record_list, DataRecord* incoming_records, lluint incoming_count) {
    DataRecord *temp = NULL;
    (*record_list)->record_count += incoming_count;
    printf("%p : %llu\n", (void*)*record_list, (*record_list)->record_count);
    if ((*record_list)->record_ptr == NULL) {
        (*record_list)->record_ptr = (DataRecord*) malloc(sizeof(DataRecord));
        (*record_list)->record_ptr->SetRecord(incoming_records[0]._record[0], incoming_records[0]._record[1], incoming_records[0]._record[2]);
        (*record_list)->record_ptr->next = NULL;
        temp = (*record_list)->record_ptr;
        incoming_records++;
        ADD_RECORDS(temp, incoming_records, incoming_count-1);
        temp = (*record_list)->record_ptr;
        while(temp != NULL) {
            temp->print();
            temp = temp->next;
        }
    } else {
        LAST_DATARECORD(temp, (*record_list));
        // cout<<"Adding to the last record "; temp->print(); cout<<" Total records: "<<incoming_count<<endl;
        ADD_RECORDS(temp, incoming_records, incoming_count);
    }
}

DataRecord* pop_record(RecordList *list) {
    DataRecord* top = NULL;
    if (list->record_count == 1) {
        top = list->record_ptr;
        list->record_ptr = NULL;
    } else if (list->record_count > 1) {
        top = list->record_ptr;
        list->record_ptr = list->record_ptr->next;
    }
    list->record_count--;
    return top;
}

DataRecord* top_record(RecordList *list) {
    if (list == NULL) {
        return NULL;
    }
    return list->record_ptr;
}

/*
 * Pass the list of sorted runs as part of structure RecordList
 */

Tree::Tree(RecordList *sorted_runs, int count_of_sorted_runs)
{
    this->tree_depth = ceil(log2(count_of_sorted_runs));
    this->total_nodes = 2 * pow(2, this->tree_depth) - 1;
    this->heap = std::vector <struct Node>(this->total_nodes);
    this->total_leaves = pow(2, this->tree_depth);
    int first_leaf_node = this->total_nodes - ((this->total_nodes - 1)/2) - 1;

    RecordList *each_run = sorted_runs;
    int current_run = 0;
    lluint ii = first_leaf_node;
    for ( ; ii < (this->total_leaves*2) - 1 ; ii++) {
        current_run++;
        this->heap[ii].current_record = NULL;
        this->heap[ii].is_empty = false;
        this->heap[ii].is_leaf = true;
        this->heap[ii].list = each_run;
        if (current_run < count_of_sorted_runs) {
            each_run+=1;
            // printf("%p\n", (void*)each_run);
        } else {
            ii++;
            break;
        }
    }
    if (ii < ((this->total_leaves*2) - 1)) {
        this->heap[ii].current_record = NULL;
        this->heap[ii].is_empty = true;
        this->heap[ii].is_leaf = true;
        this->heap[ii].list = NULL;
    }
}

/**
 * Generates a tournament tree for sorting the records
 * Conventions used:
 * * Inner nodes can contain only one record at one time, indicated
 *      by current_record
 * * Leaf nodes can contain only list (which may have 1/more records),
 *      or can be empty as well (can happen after a few runs are merged)
 *      It also stores the count of records in that particular node.
 * 
 * * By default, we will try to create a full binary tree.
 * 
 */
Tree::Tree(DataRecord *records, int record_ct, int initial_run)
{
    // TODO See if this is optimal division for fanning
    this->total_leaves = record_ct/2;
    DataRecord *current_ptr = records;
    int count_of_cols_per_row = ceil(record_ct/this->total_leaves);

	if (initial_run) {
		count_of_cols_per_row = 1;
		this->total_leaves = record_ct;
	}

	this->tree_depth = ceil(log2(this->total_leaves));
	this->total_nodes = 2 * pow(2, this->tree_depth) - 1;
	this->total_record_count = record_ct;
	this->heap = vector <struct Node>(this->total_nodes);

	int first_leaf_node = this->total_nodes - ((this->total_nodes - 1)/2) - 1;

	int current_ct = record_ct;

	// We always try to generate full binary tree at the beginning
	// (last leaf may not be balanced)
	for (lluint ii = first_leaf_node ; ii < (this->total_leaves*2) - 1 ; ii++) {
		// Leaf nodes has no current record
		this->heap[ii].current_record = NULL;
		this->heap[ii].is_empty = false;
		this->heap[ii].is_leaf = true;

        // Assign records to each row
        this->heap[ii].list = (RecordList *) malloc(sizeof(RecordList));
        this->heap[ii].list->record_ptr = (DataRecord *) malloc(sizeof(DataRecord*));
        this->heap[ii].list->record_ptr->next = NULL;
        append_to_record_list(&this->heap[ii].list, records, count_of_cols_per_row);
        this->heap[ii].list->record_count = count_of_cols_per_row;

		if (current_ct > 0) {
			current_ptr = current_ptr + count_of_cols_per_row;
			current_ct -= count_of_cols_per_row;
		} else {
			break;
		}
		// Sample calculation:
		// For 128 records, there will be 7 rows == 
		//          18 count_of_cols_per_row + 2 remaining
		// for last row, current_ct will be 2 after subtraction,
		// so we will just add it to the last row
		if (initial_run == 0) {
			if ((current_ct > 0) && (current_ct <= 2*count_of_cols_per_row)) {
				count_of_cols_per_row = current_ct;
				current_ct = -1;
			}
		}
	}
}

int Tree::capacity(int level) {
	return (1<<level);
}

/*
* Compares the record in children nodes in the tree, and pulls up the winner record.
* Runs only for the internal nodes.
* @param parent Index (in heap) of the parent record
*/
void Tree::compare_and_swap(int parent, int unused_leaves_idx) {
	int child_left = parent*2+1, child_right=parent*2+2;
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
                left_data = top_record(left_child_node->list);
                right_data = top_record(right_child_node->list);
                if ((right_data != NULL) & (left_data != NULL)) {
                    // Compare
                    if (left_data->_record[0] <= right_data->_record[0]) {
                        parent_node->current_record = pop_record(left_child_node->list);
                        CHECK_SET_EMPTY(left_child_node, child_left);
                    } else {
                        parent_node->current_record = pop_record(right_child_node->list);
                        CHECK_SET_EMPTY(right_child_node, child_right);
                    }
                } else if (left_data) {
                    parent_node->current_record = pop_record(left_child_node->list);
                    CHECK_SET_EMPTY(left_child_node, child_left);
                } else if (right_data) {
                    parent_node->current_record = pop_record(right_child_node->list);
                    CHECK_SET_EMPTY(right_child_node, child_right);
                } else {
                    parent_node->current_record = NULL;
                    // At this point, both of the left and right should have been reported as empty, so no need to update.
                }
            } else {
                // It is an internal node
                left_data = NODE_CURRENT_RECORD(left_child_node);
                right_data = NODE_CURRENT_RECORD(right_child_node);
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

/*
 * Each call runs the tree once, to generate one entry of merged run
 */
void Tree::run_tree() {
	int unused_leaves_idx = (this->total_nodes + 1) / 2 - 1 + this->total_leaves;

    // Each iteration will give one of the priority queue elements,
    // run for each of the inner nodes
    for (int inner_node_idx = this->total_nodes - pow(2, this->tree_depth) - 1;
            inner_node_idx >= 0;
            inner_node_idx--) {
        this->compare_and_swap(inner_node_idx, unused_leaves_idx);
    }
    // cout<<"The heap in iteration "<<endl;
    // this->print_heap();
    // printf("pushing %p into the generated_run, with values ", (void*) this->heap[0].current_record); this->heap[0].current_record->print(); cout<<endl; 
    this->generated_run.push_back(this->heap[0].current_record);
    this->heap[0].current_record = NULL;
}

/*
* Prints (index Empty) for empty nodes
* Prints "count -> [heap_index @ list_index :: (datarecord)]" list
* 
* For e.g.:
* (1 Empty )
* (2 Empty )
* Count: 0 ->
* Count: 1 -> [5 @ 0 :: (6, 6, 6)]
* Count: 3 -> [6 @ 0 :: (7, 7, 7)] [6 @ 1 :: (8, 8, 8)] [6 @ 2 :: (9, 9, 9)]
*/
void Tree::print_heap() {
    cout<<"Tree depth: "<<this->tree_depth+1<<", Total nodes: "<<this->total_nodes<<", Total leaves: "<<this->total_leaves<<endl;
    for (lluint ii = 0 ; ii < this->total_nodes; ii++) {
        if (!this->heap[ii].is_empty) {
            if (this->heap[ii].current_record) {
                printf("%lld :: (%d, %d, %d)\n",
                        ii, this->heap[ii].current_record->_record[0],
                        this->heap[ii].current_record->_record[1],
                        this->heap[ii].current_record->_record[2]);
            } else {
                RecordList *heap_list = this->heap[ii].list;
                DataRecord *current_record = heap_list->record_ptr;

                printf("\n(%lld (Count: %lld) -> ", ii, heap_list->record_count);
                lluint jj = 0;
                while(current_record != NULL) {
                    printf("[%lld @ %lld :: (%d, %d, %d)] ",
                        ii, jj, current_record->_record[0],
                        current_record->_record[1],
                        current_record->_record[0]);
                    current_record = current_record->next;
                    jj++;
                }
                printf(")\n");
            }
        } else {
            printf("\n(%lld Empty )\n", ii);
        }
    }
}

vector<int> Tree::get_empty_leaves() {
    vector<int> empty_leaf_idx_list;
    int first_leaf_idx = pow(2, this->tree_depth) - 1;
    for (lluint ii = first_leaf_idx; ii < this->total_nodes; ii++) {
        if ((this->heap[ii].is_empty) &&
            (this->heap[ii].list->record_ptr == NULL)) {
                empty_leaf_idx_list.push_back(ii);
        }
    }
    return empty_leaf_idx_list;
}

/*
 * Appends new sorted records at a leaf node
 */
int Tree::add_run_at_leaf(int leaf_node_index, DataRecord *record_list, lluint record_ct) {
    this->heap[leaf_node_index].is_empty = false;
    if (this->heap[leaf_node_index].list == NULL) {
        this->heap[leaf_node_index].list = create_empty_record_list(1);
    }
    append_to_record_list(&(this->heap[leaf_node_index].list), record_list, record_ct);
    return 0;
}

void Tree::spillover_run() {
    // TODO Can add spillover to HDD/SSD logic here
    // As of now, we will just empty the vector storing the merged run
    this->generated_run.clear();
}

/*
* Prints a sorted run
*/
void Tree::print_run() {
	for (auto a: this->generated_run) {
		a->print();
	}
	return;
}

Tree::~Tree ()
{
	TRACE (true);
	// delete root;
}
