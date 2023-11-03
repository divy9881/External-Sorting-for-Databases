#include "Scan.h"

ScanPlan::ScanPlan (RowCount const count) : _count (count)
{
    this->_rows = (DataRecord *)malloc(this->_count * sizeof(DataRecord));
    for (RowCount ii = 0; ii < _count; ii++) {
        this->_rows[ii].SetRecord(Random(RANDOM_INTEGER_RANGE), Random(RANDOM_INTEGER_RANGE), Random(RANDOM_INTEGER_RANGE));
    }
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

DataRecord ScanPlan::GetRecord(RowCount const rowid) const
{
    return this->_rows[rowid];
} // ScanPlan::GetRecord

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
    _plan->GetRecord(_count).print();
    ++ _count;
    return true;
} // ScanIterator::next
