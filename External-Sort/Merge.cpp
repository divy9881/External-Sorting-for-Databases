#include "Merge.h"
#include <vector>

void merge(std::vector<DataRecord>& arr, int left, int middle, int right) {
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

void mergeSort(std::vector<DataRecord>& arr, int left, int right) {
    if (left < right) {
        int middle = left + (right - left) / 2;

        mergeSort(arr, left, middle);
        mergeSort(arr, middle + 1, right);

        merge(arr, left, middle, right);
    }
}

void Merge::printDataRecords(){
    for (const DataRecord& record : this->sortedDataRecords) {
        record.print();
    }
}

Merge::Merge(DataRecord *records, int record_ct)
{
    printf("%d\n", record_ct);
    for (int i=0; i< record_ct; i++){
        records[i].print();
    }

    std::vector<DataRecord> dataRecords(records, records+record_ct);

    mergeSort(dataRecords, 0, record_ct-1);

    this->sortedDataRecords = dataRecords;

}