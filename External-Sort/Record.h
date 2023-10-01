#pragma once

#include "defs.h"

class Record
{
private:
    /* TODO 
     * Change this to strings.
     * Keeping it int for initial coding and testing
     */
    uint32_t data_col_1;
    uint32_t data_col_2;
    uint32_t data_col_3;
public:
    Record();
    Record(uint32_t data_1, uint32_t data_2, uint32_t data_3);
    ~Record();
};

