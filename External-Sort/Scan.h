#include "Iterator.h"

class ScanPlan : public Plan
{
	friend class ScanIterator;
public:
	ScanPlan (RowCount const count);
	~ScanPlan ();
	Iterator * init () const;
	DataRecord GetRecord(RowCount const rowid) const;
	RecordList * GetRecords();
private:
	RowCount const _count;
	DataRecord *_rows;
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
