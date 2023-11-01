#include "OffsetValueCode.h"

OffsetValueCode::OffsetValueCode()
{
    this->offset = -1;
    this->value = '\0';
}

OffsetValueCode::~OffsetValueCode()
{
    this->offset = INT_MIN;
    this->value = '\0';
}

// It is called only when the current record loses to the incoming
// (incoming record will be shorter/have lower value)
void OffsetValueCode::create_or_update_OVC_int(int current, int winner_key)
{
    std::string current_record = std::to_string(current);
    std::string winner_record = std::to_string(winner_key);

    // Traverse over winner record length (it will be shorter/lower value)
    for (long unsigned ii = 0 ; ii < winner_record.length(); ii++) {
        if (current_record[ii] == winner_record[ii]) {
            continue;
        } else {
            this->offset = ii;
            this->value = current_record[ii];
            break;
        }
    }
}

void OffsetValueCode::create_or_update_OVC_str(std::string current, std::string winner_key)
{
    // Traverse over winner record length (it will be shorter/lower value)
    for (unsigned long ii = 0 ; ii < winner_key.length(); ii++) {
        if (current[ii] == winner_key[ii]) {
            continue;
        } else {
            this->offset = ii;
            this->value = current[ii];
            break;
        }
    }
}

