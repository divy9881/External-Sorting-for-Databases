#pragma once

#include "defs.h"

class DataRecord
{
public:
    DataRecord (int col1, int col2, int col3);
    DataRecord (const DataRecord& record);
    ~DataRecord ();
    void print();

private:
    int _record[3];
}; // class DataRecord