#include "SortTrace.h"

SortTrace::SortTrace(char trace_file[256 + 1])
{
    string trace_dir = "trace/";
    string tracefile = string(trace_file);

    tracefile = trace_dir + tracefile;

    strcpy(this->trace_file, tracefile.c_str());

    this->truncate_trace();

    mkdir(trace_dir.c_str(), 0777);
}

void SortTrace::append_trace(string trace)
{
    fstream tracefile;
    string trace_file = string(this->trace_file);

    tracefile.open(trace_file, ios_base::app);
    if (!tracefile.is_open())
        return;

    tracefile << trace << endl;

    tracefile.close();

    return;
}

void SortTrace::truncate_trace()
{
    struct stat sb;

    if (stat(this->trace_file, &sb))
        return;

    remove(this->trace_file);

    return;
}