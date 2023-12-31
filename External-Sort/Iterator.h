#pragma once

#include "defs.h"
#include "setupConfig.h"
#include "DataRecord.h"

typedef uint64_t RowCount;

class Plan
{
	friend class Iterator;
public:
	Plan ();
	virtual ~Plan ();
	virtual class Iterator * init () const = 0;
private:
}; // class Plan

class Iterator
{
public:
	Iterator ();
	virtual ~Iterator ();
	void run ();
	virtual bool next () = 0;
	static pair<bool, lluint> verify_sort_order ();
private:
	RowCount _count;
}; // class Iterator
