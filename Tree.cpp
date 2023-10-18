#include "Tree.h"

Tree::Tree(std::vector<DataRecord>& records) 
{
    this->records = records;
}

Node* Tree::getRoot()
{
    root = buildTournamentTree(records, 0, records.size() - 1);
    return root;
}
Node* Tree::buildTournamentTree(const std::vector<DataRecord>& records, int start, int end) 
{
    if (start == end) 
    {
        return new Node(records[start]);
    }

    int mid = start + (end - start) / 2;
    Node* left = buildTournamentTree(records, start, mid);
    Node* right = buildTournamentTree(records, mid + 1, end);

    Node* winnerNode = (left->data < right->data) ? left : right;
    Node* loserNode = (winnerNode == left) ? right : left;

    Node* rootNode = new Node(loserNode->data);
    rootNode->left = loserNode;
    rootNode->right = winnerNode;
    return rootNode;
}

DataRecord Tree::findWinner() 
{
    Node* current = root;
    while (current->right != nullptr) {
        current = current->right;
    }
    return current->data;
}

void Tree:: getSortedDataRecordsCaller() 
{
    getSortedDataRecords(root, sortedRecords);
}

void Tree::getSortedDataRecords(Node* rootNode, std::set<DataRecord>& sortedDataRecords) {
    if (rootNode == nullptr) {
        return;
    }
    getSortedDataRecords(rootNode->left, sortedDataRecords);
    sortedDataRecords.insert(rootNode->data);
    getSortedDataRecords(rootNode->right, sortedDataRecords);
}

Tree::~Tree ()
{
	TRACE (true);
	delete root;
}
