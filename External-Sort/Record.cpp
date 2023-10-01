#include "Record.h"

Record::Record()
{
    TRACE (false);
}

Record::Record(uint32_t data_1, uint32_t data_2, uint32_t data_3)
{
    this->data_col_1 = data_1;
    this->data_col_2 = data_2;
    this->data_col_3 = data_3;
}

Record::~Record()
{
    TRACE (true);
}

// TODO Add required functions (if any)