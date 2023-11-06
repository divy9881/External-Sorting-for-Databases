#pragma once

#include "defs.h"

class OffsetValueCode {
	public:
		OffsetValueCode();
		~OffsetValueCode();
		void populate_ovc_int(lluint current, lluint winner_key);
		void populate_ovc_str(string current, string winner_key);
		void print();
	private:
		suint ovc;
		string rel;
		bool isempty;
		friend class Tree;
		friend class DataRecord;
		bool operator<(OffsetValueCode const&obj);
		bool operator>(OffsetValueCode const&obj);
		bool operator==(OffsetValueCode const&obj);
};
