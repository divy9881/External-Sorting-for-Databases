#include "Scan.h"
#include "sortParams.h"

class SortRecords {
	public:
		SortRecords(lluint num_records);
		void run();
	private:
		lluint num_records;
};

