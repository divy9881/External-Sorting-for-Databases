#pragma once

#include "defs.h"

class SortTrace {
    public:
        SortTrace(char trace_file[256 + 1]);
        void append_trace(string trace);
        void truncate_trace();
    private:
        char trace_file[256 + 1];
};