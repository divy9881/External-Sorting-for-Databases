#include "DataRecord.h"
#include <vector>
#include <algorithm>

DataRecord::DataRecord (int col1, int col2, int col3)
{
    this->_record[0] = col1;
    this->_record[1] = col2;
    this->_record[2] = col3;
    TRACE (true);
} // DataRecord::DataRecord (int col1, int col2, int col3)

DataRecord::DataRecord (const DataRecord& record)
{
    this->_record[0] = record._record[0];
    this->_record[1] = record._record[1];
    this->_record[2] = record._record[2];
    TRACE (true);
} // DataRecord::DataRecord (const DataRecord& record)

DataRecord::~DataRecord ()
{
    this->_record[0] = INT_MIN;
    this->_record[1] = INT_MIN;
    this->_record[2] = INT_MIN;
    TRACE (true);
} // DataRecord::~DataRecord

void DataRecord::print ()
{
    printf("%d %d %d\n", this->_record[0], this->_record[1], this->_record[2]);
    TRACE (true);
} // DataRecord::~DataRecord

bool DataRecord::operator<(const DataRecord& other) const
{
    return _record[0] < other._record[0];
}

bool DataRecord::operator==(const DataRecord& other) const 
{
    return std::equal(std::begin(_record), std::end(_record), std::begin(other._record));
}
