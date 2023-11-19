#include "Scan.h"
#include "databaseConfig.h"

ScanPlan::ScanPlan (RowCount const count) : _count (count)
{
	this->_rows = new DataRecord[this->_count];
	for (RowCount ii = 0; ii < _count; ii++) {
		this->_rows[ii].SetRecord(Random(DOMAIN_COL_VALUE), Random(DOMAIN_COL_VALUE), Random(DOMAIN_COL_VALUE));
	}
	TRACE (true);
} // ScanPlan::ScanPlan

ScanPlan::~ScanPlan ()
{
	delete []this->_rows;
	TRACE (true);
} // ScanPlan::~ScanPlan

Iterator * ScanPlan::init () const
{
	TRACE (true);
	return new ScanIterator (this);
} // ScanPlan::init

DataRecord ScanPlan::GetRecord (RowCount const rowid) const
{
	return this->_rows[rowid];
} // ScanPlan::GetRecord

RecordList * ScanPlan::GetRecords ()
{
	RecordList *record_list = new RecordList;
	DataRecord *records = new DataRecord[this->_count];

	for (lluint ii = 0 ; ii < this->_count ; ii++) {
		records[ii] = this->GetRecord(ii);
	}

	record_list->record_ptr = records;
	record_list->record_count = this->_count;

	return record_list;
} // ScanPlan::GetRecords

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
