#pragma once

#include <string>
#include "defs.h"

class OffsetValueCode {
    public:
        int offset;
        char value;

        OffsetValueCode();
        ~OffsetValueCode();
        void create_or_update_OVC_int(int current, int winner_key);
        void create_or_update_OVC_str(std::string current, std::string winner_key);
};
