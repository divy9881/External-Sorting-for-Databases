#pragma once

#include "defs.h"

class DataRecord
{
public:
    DataRecord (int col1, int col2, int col3);
    DataRecord (const DataRecord& record);
    DataRecord ();
    ~DataRecord ();
    void print();
    bool operator<(const DataRecord& other) const;
    bool operator==(const DataRecord& other) const;

// private:
    int _record[3];
}; // class DataRecord