#include "DataRecord.h"
#include <vector>

class Merge
{   private:
    std::vector<DataRecord> sortedDataRecords;
    public:
    Merge(DataRecord *, int);
    printDataRecords();
};