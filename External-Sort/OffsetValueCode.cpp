#include "OffsetValueCode.h"
#include "sortParams.h"

OffsetValueCode::OffsetValueCode()
{
	this->ovc = 0;
	this->rel = "\0";
}

OffsetValueCode::~OffsetValueCode()
{
	this->ovc = 0;
	this->rel = "\0";
}

// It is called only when the current record loses to the incoming
// (incoming record will be shorter/have lower value)
void OffsetValueCode::populate_ovc_int(lluint current, lluint winner)
{
	string current_record = to_string(current);
	string winner_record = to_string(winner);

	this->populate_ovc_str(current_record, winner_record);
}

void OffsetValueCode::populate_ovc_str(std::string current, std::string winner)
{
	int current_length = current.length();
	int winner_length = winner.length();
	int arity = 0;

	if (current_length > winner_length) {
		int num_of_zeroes = current_length - winner_length;
		string zeroes = "";

		for (int ii = 0; ii < num_of_zeroes; ii++) {
			zeroes += "0";
		}
		winner = zeroes + winner;
	} else {
		int num_of_zeroes = winner_length - current_length;
		string zeroes = "";

		for (int ii = 0; ii < num_of_zeroes; ii++) {
			zeroes += "0";
		}
		current = zeroes + current;
	}

	arity = winner.length();

	// Traverse over winner record length (it will be shorter/lower value)
	for (lluint ii = 0; ii < winner.length(); ii++) {
		if (current[ii] == winner[ii]) {
			continue;
		} else {
			this->ovc = (arity - ii) * OVC_DOMAIN + (current[ii] - '0');
			this->rel = winner;
			break;
		}
	}
}

void OffsetValueCode::print()
{
	cout<<"OVC: "<<this->ovc<<" Rel: "<<this->rel<<endl;
}
