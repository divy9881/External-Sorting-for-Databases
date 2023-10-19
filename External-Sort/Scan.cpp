#include "Scan.h"

ScanPlan::ScanPlan (RowCount const count) : _count (count)
{
    this->_rows = (DataRecord *)malloc(this->_count * sizeof(DataRecord));
    TRACE (true);
} // ScanPlan::ScanPlan

ScanPlan::~ScanPlan ()
{
    free(this->_rows);
    TRACE (true);
} // ScanPlan::~ScanPlan

Iterator * ScanPlan::init () const
{
    TRACE (true);
    return new ScanIterator (this);
} // ScanPlan::init

ScanIterator::ScanIterator (ScanPlan const * const plan) :
	_plan (plan), _count (0)
{
    TRACE (true);
} // ScanIterator::ScanIterator

ScanIterator::~ScanIterator ()
{
    TRACE (true);
    traceprintf ("produced %lu of %lu rows\n",
            (unsigned long) (_count),
            (unsigned long) (_plan->_count));
} // ScanIterator::~ScanIterator

bool ScanIterator::next ()
{
    TRACE (true);

    if (_count >= _plan->_count)
        return false;

    DataRecord row = DataRecord(Random(RANDOM_INTEGER_RANGE), Random(RANDOM_INTEGER_RANGE), Random(RANDOM_INTEGER_RANGE));
    this->_plan->_rows[this->_count] = row;
    row.print();
    ++ _count;
    return true;
} // ScanIterator::next