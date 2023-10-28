#include "DataRecord.h"
#include <vector>

class Merge {
private:
    std::vector<DataRecord> sortedDataRecords;

public:
    Merge(DataRecord*, int);
    void printDataRecords();
    void mergeSort(std::vector<DataRecord>& arr, int left, int right);
    void merge(std::vector<DataRecord>& arr, int left, int middle, int right);
    void sortDataRecords();
};