#include "Iterator.h"

Plan::Plan ()
{
	TRACE(ENABLE_TRACE);
} // Plan::Plan

Plan::~Plan ()
{
	TRACE(ENABLE_TRACE);
} // Plan::~Plan

Iterator::Iterator () : _count (0)
{
	TRACE(ENABLE_TRACE);
} // Iterator::Iterator

Iterator::~Iterator ()
{
	TRACE(ENABLE_TRACE);
} // Iterator::~Iterator

void Iterator::run ()
{
	TRACE(ENABLE_TRACE);

	while (next ())  ++ _count;

	traceprintf ("entire plan produced %lu rows\n",
			(unsigned long) _count);
} // Iterator::run
