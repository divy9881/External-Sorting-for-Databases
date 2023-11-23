#include "Filter.h"

FilterPlan::FilterPlan (Plan * const input) : _input (input)
{
	TRACE(ENABLE_TRACE);
} // FilterPlan::FilterPlan

FilterPlan::~FilterPlan ()
{
	TRACE(ENABLE_TRACE);
	delete _input;
} // FilterPlan::~FilterPlan

Iterator * FilterPlan::init () const
{
	TRACE(ENABLE_TRACE);
	return new FilterIterator (this);
} // FilterPlan::init

FilterIterator::FilterIterator (FilterPlan const * const plan) :
	_plan (plan), _input (plan->_input->init ()),
	_consumed (0), _produced (0)
{
	TRACE(ENABLE_TRACE);
} // FilterIterator::FilterIterator

FilterIterator::~FilterIterator ()
{
	TRACE(ENABLE_TRACE);

	delete _input;

	traceprintf ("produced %lu of %lu rows\n",
			(unsigned long) (_produced),
			(unsigned long) (_consumed));
} // FilterIterator::~FilterIterator

bool FilterIterator::next ()
{
	TRACE(ENABLE_TRACE);

	do
	{
		if ( ! _input->next ())  return false;
		++ _consumed;
	} while (_consumed % 2 == 0);

	++ _produced;
	return true;
} // FilterIterator::next
