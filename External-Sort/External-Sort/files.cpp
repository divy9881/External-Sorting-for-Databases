#include <iostream>
#include <vector>
#include <algorithm>

class DataRecord {
public:
    DataRecord(int col1, int col2, int col3) {
        _record[0] = col1;
        _record[1] = col2;
        _record[2] = col3;
    }

    DataRecord(const DataRecord& record) {
        for (int i = 0; i < 3; i++) {
            _record[i] = record._record[i];
        }
    }

    void print() const {
        std::cout << _record[0] << " " << _record[1] << " " << _record[2] << std::endl;
    }

    bool operator<(const DataRecord& other) const {
        // Compare DataRecord objects based on col1
        return _record[0] < other._record[0];
    }

private:
    int _record[3];
};

struct Node {
    DataRecord data;
    Node* left;
    Node* right;

    Node(const DataRecord& val) : data(val), left(nullptr), right(nullptr) {}
};

Node* buildTournamentTree(const std::vector<DataRecord>& records, int start, int end) {
    if (start == end) {
        return new Node(records[start]);
    }

    int mid = start + (end - start) / 2;
    Node* left = buildTournamentTree(records, start, mid);
    Node* right = buildTournamentTree(records, mid + 1, end);

    Node* winner = (left->data < right->data) ? left : right;
    Node* loser = (winner == left) ? right : left;

    Node* root = new Node(winner->data);
    root->left = winner;
    root->right = loser;

    return root;
}

DataRecord findWinner(Node* root) {
    return root->data;
}

int main() {
    std::vector<DataRecord> records = {
        DataRecord(45, 2, 100),
        DataRecord(11, 55, 30),
        DataRecord(600, 43, 200),
        DataRecord(9, 20, 77)
    };

    Node* root = buildTournamentTree(records, 0, records.size() - 1);

    DataRecord winner = findWinner(root);

    std::cout << "Winner based on col1:" << std::endl;
    winner.print();

    return 0;
}
