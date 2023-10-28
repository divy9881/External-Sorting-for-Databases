#include "Merge.h"

void Merge::merge(std::vector<DataRecord>& arr, int left, int middle, int right) {
    int n1 = middle - left + 1;
    int n2 = right - middle;

    std::vector<DataRecord> left_arr(n1);
    std::vector<DataRecord> right_arr(n2);

    for (int i = 0; i < n1; i++) {
        left_arr[i] = arr[left + i];
    }
    for (int j = 0; j < n2; j++) {
        right_arr[j] = arr[middle + 1 + j];
    }

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (left_arr[i] <= right_arr[j]) {
            arr[k] = left_arr[i];
            i++;
        } else {
            arr[k] = right_arr[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = left_arr[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = right_arr[j];
        j++;
        k++;
    }
}

void Merge::mergeSort(std::vector<DataRecord>& arr, int left, int right) {
    if (left < right) {
        int middle = left + (right - left) / 2;

        mergeSort(arr, left, middle);
        mergeSort(arr, middle + 1, right);

        merge(arr, left, middle, right);
    }
}

void Merge::sortDataRecords() {
    mergeSort(sortedDataRecords, 0, sortedDataRecords.size() - 1);
}

void Merge::printDataRecords() {
    for (const DataRecord& record : sortedDataRecords) {
        record.print();  // Call the print function on each DataRecord
    }
}

Merge::Merge(DataRecord* records, int record_ct) {
    for (int i = 0; i < record_ct; i++) {
        sortedDataRecords.push_back(records[i]);
    }
    sortDataRecords();
}
