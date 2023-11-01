#pragma once

#include "defs.h"

class OffsetValueCode {
    public:
        suint ovc;
        string rel;

        OffsetValueCode();
        ~OffsetValueCode();
        void populate_ovc_int(int current, int winner_key);
        void populate_ovc_str(string current, string winner_key);
};
