#include "Sort.h"

SortPlan::SortPlan (Plan * const input) : _input (input)
{
	TRACE(ENABLE_TRACE);
} // SortPlan::SortPlan

SortPlan::~SortPlan ()
{
	TRACE(ENABLE_TRACE);
	delete _input;
} // SortPlan::~SortPlan

Iterator * SortPlan::init () const
{
	TRACE(ENABLE_TRACE);
	return new SortIterator (this);
} // SortPlan::init

SortIterator::SortIterator (SortPlan const * const plan) :
	_plan (plan), _input (plan->_input->init ()),
	_consumed (0), _produced (0)
{
	TRACE(ENABLE_TRACE);

	while (_input->next ())  ++ _consumed;
	delete _input;

	traceprintf ("consumed %lu rows\n",
			(unsigned long) (_consumed));
} // SortIterator::SortIterator

SortIterator::~SortIterator ()
{
	TRACE(ENABLE_TRACE);

	traceprintf ("produced %lu of %lu rows\n",
			(unsigned long) (_produced),
			(unsigned long) (_consumed));
} // SortIterator::~SortIterator

bool SortIterator::next ()
{
	TRACE(ENABLE_TRACE);

	if (_produced >= _consumed)  return false;

	++ _produced;
	return true;
} // SortIterator::next