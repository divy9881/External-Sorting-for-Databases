#include <iostream>
#include <vector>
#include <limits>
#include "DataRecord.h"
#include <set>
using namespace std;

struct Node {
    DataRecord data;
    Node* left;
    Node* right;
    Node(const DataRecord& val) : data(val), left((Node*)nullptr), right((Node*)nullptr) {}
};

class Tree
{
private:
    std::vector<DataRecord> records;
    std::set<DataRecord> sortedRecords;
    DataRecord winner;
    Node* root;
public:
    Tree(std::vector<DataRecord>& records);
    Node* getRoot();
    Node* buildTournamentTree(const std::vector<DataRecord>& records, int start, int end);
    DataRecord findWinner();
    void getSortedDataRecordsCaller();
    void getSortedDataRecords(Node* root, std::set<DataRecord>& result);
    ~Tree();
};
