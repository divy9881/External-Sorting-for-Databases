#include "StorageDevice.h"

extern SortTrace trace;

/*
 * Constructor to initialize the StorageDevice Object
 * Params:
 * device_path - Relative/absolute path to the device
 * total_space - Total available space for storage on the device 
 */
StorageDevice::StorageDevice(string device_path, lluint total_space, uint col_value_length)
{
	this->device_path = device_path;
	this->free_space = total_space;
	this->total_space = total_space;
	this->run_offsets = new lluint[MAX_RUNS];
	this->total_reads = 0;
	this->total_writes = 0;
	this->col_value_length = col_value_length;

	mkdir(this->device_path.c_str(), 0777);

	for(uint ii = 0 ; ii < MAX_RUNS ; ii++)
	{
		this->run_offsets[ii] = 0;
	}
}

StorageDevice::~StorageDevice()
{
	delete [] this->run_offsets;
}

/*
 * Get the last run(sorted serially on run-number) stored on a StorageDevice
 */
int StorageDevice::get_last_run()
{
	int n;
	uint run;
	struct dirent **namelist;

	n = scandir(this->device_path.c_str(), &namelist, 0, alphasort);

	if (n <= 2) {
		run = -1;
		goto out;
	} else {
		if (namelist[n - 1]->d_name[0] == 't') {
			run = -1;
			goto out;
		}
		sscanf((char *)&namelist[n - 1]->d_name[4], "%u", &run);
	}

out:
	for (uint ii = 0; ii < (uint)n; ii++)
	{
		free(namelist[ii]);
	}
	free(namelist);

	return run;
}

/*
 * Get total number of runs present on the StorageDevice
 */
uint StorageDevice::get_num_runs()
{
	uint n;
	struct dirent **namelist;
	uint count = 0;

	n = scandir(this->device_path.c_str(), &namelist, 0, alphasort);

	for (uint ii = 2; ii < n ; ii++)
	{
		if (namelist[ii]->d_name[0] == 't') {
			continue;
		}

		count += 1;
	}

	for (uint ii = 0; ii < (uint)n; ii++)
	{
		free(namelist[ii]);
	}
	free(namelist);

	return count;
}

/*
 * Get the number of records present in a all the runs on a StorageDevice
 */
int StorageDevice::get_num_records()
{
	uint n;
	lluint count = 0;
	struct dirent **namelist;

	n = scandir(this->device_path.c_str(), &namelist, 0, alphasort);

	for (uint ii = 2; ii < n; ii++)
	{
		uint run;

		if (namelist[ii]->d_name[0] == 't') {
			continue;
		}

		sscanf((char *)&namelist[ii]->d_name[4], "%u", &run);

		count += this->get_run_num_records(run);
	}

	for (uint ii = 0; ii < (uint)n; ii++)
	{
		free(namelist[ii]);
	}
	free(namelist);

	return count;
}

/*
 * Get the free available space on the device in bytes
 */
lluint StorageDevice::get_free_space()
{
	return this->free_space;
}

/*
 * To persist a sorted run to the StorageDevice either to new run-file or to specified run-file
 * Params:
 * run_bit - Specifies whether to create a new run-file or to spill to a existing run-file
 * run - Specifies the run-number of the run to which the records are to be spilled
 * records - List of DataRecord(s) which are to be spilled to the StorageDevice
 */
void StorageDevice::spill_run(char run_bit, uint run, vector<DataRecord*> records)
{
	uint on_disk_record_size = 0;
	string run_path;
	string trace_str;
	lluint time_spent_us;
	clock_t begin_time;

	if (records.size()) {
		on_disk_record_size = ON_DISK_RECORD_SIZE(this->col_value_length);
	}

	if (run_bit == 'n') {
		int last_run = this->get_last_run();
		run_path = this->device_path + "/run_" + to_string(last_run + 1);
	} else if (run_bit == 't') {
		string trace_str = "STATE -> SPILL_RUNS_" + this->device_path + ": Spill sorted runs to the " + this->device_path + " device";

		trace.append_trace(trace_str);
		run_path = this->device_path + "/temp_run";
	} else {
		run_path = this->device_path + "/run_" + to_string(run);
	}

	begin_time = clock();	
	this->spill_run_to_disk(run_path, records);
	time_spent_us = float(clock() - begin_time) * 1000 * 1000 / CLOCKS_PER_SEC;

	this->free_space -= records.size() * on_disk_record_size;

	this->total_writes += 1;

	trace_str += "ACCESS -> A write to " + this->device_path;
	trace_str += " was made with size " + to_string(records.size() * on_disk_record_size) + " bytes";
	trace_str += " and latency " + to_string(time_spent_us) + "us";

	trace.append_trace(trace_str);
}

/*
 * To persist a sorted runs to the StorageDevice
 * Params:
 * record_lists - List of RecordList(s) which represents multiple number of sorted runs
 */
void StorageDevice::spill_runs(vector<RecordList *> record_lists)
{
	string trace_str = "STATE -> SPILL_RUNS_" + this->device_path + ": Spill sorted runs to the " + this->device_path + " device";

	trace.append_trace(trace_str);

	for (uint ii = 0 ; ii < record_lists.size() ; ii++) {
		vector<DataRecord*> records;
		RecordList *list = record_lists[ii];

		while(!list->record_ptr.empty()) {
			DataRecord *temp = new DataRecord(list->record_ptr.front());
			records.push_back(temp);
			list->record_ptr.pop_front();
		}

		this->spill_run('n', 0, records);
		for (lluint ii = 0; ii < records.size(); ii++) {
			delete records[ii];
		}
	}

	for (uint ii = 0 ; ii < record_lists.size() ; ii++) {
		// delete [] record_lists[ii]->record_ptr;
		delete record_lists[ii];
	}
}

/*
 * Get page of records from a run-file
 * Params:
 * run - Specifies the run-number of the run from which a page of records is to be fetched
 * num_records - Specifies the number of records to be fetched as part of a page
 */
vector<DataRecord> StorageDevice::get_run_page(uint run, uint num_records)
{
	lluint time_spent_us;
	string run_path = this->device_path + "/run_" + to_string(run);
	string trace_str;
	clock_t begin_time;
	vector<DataRecord> records;

	begin_time = clock();
	records = this->get_run_page_from_disk(run_path, &this->run_offsets[run], num_records);
	time_spent_us = float(clock() - begin_time) * 1000 * 1000 / CLOCKS_PER_SEC;

	this->total_reads += 1;

	trace_str += "ACCESS -> A read to " + this->device_path;
	trace_str += " was made with size " + to_string(num_records * ON_DISK_RECORD_SIZE(this->col_value_length)) + " bytes";
	trace_str += " and latency " + to_string(time_spent_us) + "us";

	trace.append_trace(trace_str);

	return records;
}

/*
 * Get pages of records from each run persisted on the StorageDevice
 * Params:
 * num_records - Specifies the number of records to be fetched as part of each page of the runs
 */
pair<vector<RecordList *>, lluint> StorageDevice::get_run_pages(uint num_records)
{
	uint n;
	lluint count = 0;
	struct dirent **namelist;
	vector<RecordList *> record_lists;
	pair <vector<RecordList *>, lluint> p;
	string trace_str = "STATE -> READ_RUN_PAGES_" + this->device_path + ": Read sorted run pages from the " + this->device_path + " device";

	trace.append_trace(trace_str);

	n = scandir(this->device_path.c_str(), &namelist, 0, alphasort);

	for (uint ii = 2 ; ii < n ; ii++) {
		uint run;
		vector<DataRecord> records;
		// DataRecord *record_objs;
		RecordList *list = new RecordList;

		sscanf((char *)&namelist[ii]->d_name[4], "%u", &run);

		records = this->get_run_page(run, num_records);

		// record_objs = new DataRecord[records.size()];
		for (uint jj = 0 ; jj < records.size() ; jj++) {
			list->record_ptr.push_back(records[jj]);
		}

		// list->record_ptr = record_objs;
		list->record_count = records.size();

		count += records.size();
		record_lists.push_back(list);

		records.erase(records.begin(), records.end());
	}

	p.first = record_lists;
	p.second = count;

	for (uint ii = 0; ii < (uint)n; ii++)
	{
		free(namelist[ii]);
	}
	free(namelist);

	return p;
}

/*
 * Commit the temporary run as a valid run-file
 */
void StorageDevice::commit_temp_run()
{
	int last_run = this->get_last_run();
	string temp_run_path = this->device_path + "/temp_run";
	string committed_run_path = this->device_path + "/run_" + to_string(last_run + 1);

	if (access(temp_run_path.c_str(), F_OK) == 0) {
		rename(temp_run_path.c_str(), committed_run_path.c_str());
	}

	return;
}

/*
 * Deletes all the runs present on the StorageDevice
 */
void StorageDevice::truncate_device()
{
	truncate_all_runs();

	this->free_space = this->total_space;

	for(uint ii = 0 ; ii < MAX_RUNS ; ii++)
	{
		this->run_offsets[ii] = 0;
	}
}

/*
 * Print device read and write statistics
 */
void StorageDevice::get_device_access_stats()
{
	cout << "Number of reads on the device: " << this->total_reads << endl;
	cout << "Number of writes on the device: " << this->total_writes << endl;
}

/*
 * Implementation of get_num_records()
 * Should never be called by the user of StorageDevice object directly
 * Params:
 * run - Specifies the run-number of the run-file for which the count of records are to be returned
 */
lluint StorageDevice::get_run_num_records(uint run)
{
	fstream runfile;
	lluint count = 0;
	string run_path = this->device_path + "/run_" + to_string(run);
	char *run_page = new char[ON_DISK_RECORD_SIZE(this->col_value_length) + 1];

	runfile.open(run_path, ios::in);
	if (!runfile.is_open())
		return -1;

	runfile.seekg(0, ios::beg);

	do
	{
		runfile.get(run_page, ON_DISK_RECORD_SIZE(this->col_value_length) + 1);
		if (strlen(run_page) == ON_DISK_RECORD_SIZE(this->col_value_length)) {
			count += 1;
		} else {
			break;
		}
	} while (true);

	runfile.close();

	delete [] run_page;

	return count;
}

/*
 * Implementation of spill_run()
 * Should never be called by the user of StorageDevice object directly
 * Params:
 * run_path - Specifies the path of the run-file to which records are to be spilled
 * records - List of records to be spilled to a run-file
 */
void StorageDevice::spill_run_to_disk(string run_path, vector<DataRecord*> records)
{
	fstream runfile;
	string str_records = "";

	runfile.open(run_path, ios_base::app);
	if (!runfile.is_open())
		return;

	for (uint ii = 0 ; ii < records.size() ; ii++) {
		DataRecord *record = records[ii];
		string str_record = record->GetRecord();
		str_records += str_record + STORAGE_RECORD_DELIMITER;
	}
	runfile << str_records;

	runfile.close();
	return;
}

/*
 * Implementation of get_run_page()
 * Should never be called by the user of StorageDevice object directly
 * Params:
 * run_path - Specifies the path of the run-file from which records are to be paged-in
 * offset - Specifies the current offset of a run-file from where the pages should be fetched
 * num_records - Specifies the number of records to be fetched as part of the page from the run
 */
vector<DataRecord> StorageDevice::get_run_page_from_disk(string run_path, lluint *offset, uint num_records)
{
	fstream runfile;	
	vector<DataRecord> records;
	lluint start_pos = 0;
	string col_value1, col_value2, col_value3;
	string record_delimiter(STORAGE_RECORD_DELIMITER);
	string column_delimiter(STORAGE_COLUMN_DELIMITER);
	uint record_size = ON_DISK_RECORD_SIZE(this->col_value_length);
	char *run_page = new char[num_records * record_size + 1];

	runfile.open(run_path, ios::in);
	if (!runfile.is_open())
		return records;

	runfile.seekg(*offset, ios::beg);

	runfile.get(run_page, num_records * record_size + 1);
	runfile.close();
	*offset += strlen(run_page);

	/*
	 * Remove file if we have exhausted all the records
	 * from the run file
	 */
	if (strlen(run_page) != (num_records * record_size)) {
		remove(run_path.c_str());
		*offset = 0;
	}

	string s(run_page);
	/*
	 * Iterating over records
	 */
	while (start_pos < strlen(run_page))
	{
		string record_str;

		record_str = s.substr(start_pos, record_size);
		/* 
		 * For Column Value 1
		 */
		col_value1 = record_str.substr(0, this->col_value_length);
		/*
		 * For Column Value 2
		 */
		col_value2 = record_str.substr(this->col_value_length + 1, this->col_value_length);
		/*
		 * For Column Value 3
		 */
		col_value3 = record_str.substr(this->col_value_length * 2 + 2, this->col_value_length);

		DataRecord record = DataRecord(col_value1, col_value2, col_value3, this->col_value_length);
		records.push_back(record);
		start_pos += record_size;
	}

	delete[] run_page;

	return records;
}

/*
 * Implementation of truncate_device()
 * Should never be called by the user of StorageDevice object directly
 */
int StorageDevice::truncate_all_runs()
{
	struct stat sb;
	string device_path = this->device_path;
	DIR *device = opendir(device_path.c_str());
	struct dirent *next_run;
	char runpath[257];

	if (stat(device_path.c_str(), &sb))
		return 0;

	while ((next_run = readdir(device)) != NULL)
	{
		sprintf(runpath, "%s/%s", device_path.c_str(), next_run->d_name);
		remove(runpath);
	}
	closedir(device);

	return 0;
}