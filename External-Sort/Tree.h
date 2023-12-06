#pragma once

#include "defs.h"
#include "DataRecord.h"

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
	llint level;
	Node() {
		current_record = NULL;
		is_empty = true;
		is_leaf = false;
		list = NULL;
		level = 0;
	}
	// Node(const DataRecord& val) : data(val), left((Node*)nullptr), right((Node*)nullptr) {}
};

class Tree
{
	private:
		struct Node *heap;
		vector<DataRecord *> generated_run;
		lluint total_leaves, total_nodes, tree_depth;
		lluint total_record_count;
	public:
		// Constructor
		Tree();
		Tree(DataRecord *, llint, llint); // Used for sorting of individual records
		Tree(vector<RecordList *>); // Used to generate tree with sorted runs

		// Utility functions
		Node* getRoot();
		llint capacity(llint level);
		struct Node leaf(llint index, llint slot);
		struct Node parent(llint slot);
		void print_heap();
		void print_run();

		// Tournament tree functions
		void run_tree();
		void run_tournament(llint parent, llint unused_leaf_idx);
		vector<llint> get_empty_leaves();
		llint add_run_at_leaf(llint leaf_node_idx, DataRecord *sorted_run, llint number_of_records);
		void spillover_run();
		vector<DataRecord*>  get_generated_run();
		
		// Deconstructor
		~Tree();
};
