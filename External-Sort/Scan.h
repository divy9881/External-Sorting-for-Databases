#pragma once

#include "Iterator.h"

class ScanPlan : public Plan
{
	friend class ScanIterator;
public:
	ScanPlan (RowCount const count, uint col_value_length);
	~ScanPlan ();
	Iterator * init () const;
	DataRecord GetRecord(RowCount const rowid) const;
	RecordList * GetRecords();
private:
	RowCount const _count;
	DataRecord *_rows;
	uint col_value_length;
}; // class ScanPlan

class ScanIterator : public Iterator
{
public:
	ScanIterator (ScanPlan const * const plan);
	~ScanIterator ();
	bool next ();
private:
	ScanPlan const * const _plan;
	RowCount _count;
}; // class ScanIterator
