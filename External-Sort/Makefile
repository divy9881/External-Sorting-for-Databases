CPPOPT=-g -Og -D_DEBUG
# -O2 -Os -Ofast
# -fprofile-generate -fprofile-use
CPPFLAGS=$(CPPOPT) -Wall -ansi -pedantic -std=c++11
# -Wparentheses -Wno-unused-parameter -Wformat-security
# -fno-rtti -std=c++11 -std=c++98

# documents and scripts
DOCS=Tasks.txt
SCRS=

# headers and code sources
HDRS=	defs.h setupConfig.h\
		Iterator.h Scan.h Filter.h Sort.h DataRecord.h\
		Tree.h\
		StorageDevice.h SSDDevice.h HDDDevice.h\
		SortRecords.h Table.h\
SRCS=	defs.cpp Assert.cpp Test.cpp ExternalSort.cpp\
		Iterator.cpp Scan.cpp Filter.cpp Sort.cpp DataRecord.cpp\
		Tree.cpp\
		StorageDevice.cpp\
		SortRecords.cpp Table.cpp\

# compilation targets
OBJS=	defs.o Assert.o Test.o Table.o\
		Iterator.o Scan.o Filter.o Sort.o DataRecord.o\
		Tree.o\
		StorageDevice.o\
		SortRecords.o\

# compilation targets
EXTERNAL_SORT_OBJS=	defs.o Assert.o ExternalSort.o Table.o\
		Iterator.o Scan.o Filter.o Sort.o DataRecord.o\
		Tree.o\
		StorageDevice.o\
		SortRecords.o\

# RCS assists
REV=-q -f
MSG=no message

# default target
#
Test.exe : Makefile $(OBJS)
	g++ $(CPPFLAGS) -o Test.exe $(OBJS)

ExternalSort.exe : Makefile $(EXTERNAL_SORT_OBJS)
	g++ $(CPPFLAGS) -o ExternalSort.exe $(EXTERNAL_SORT_OBJS)

trace : Test.exe Makefile
	@date > trace
	./Test.exe >> trace
	@size -t Test.exe $(OBJS) | sort -r >> trace

$(OBJS) : Makefile defs.h setupConfig.h
ExternalSort.o : Iterator.h Scan.h Filter.h Sort.h DataRecord.o
Test.o : Iterator.h Scan.h Filter.h Sort.h DataRecord.o
Iterator.o Scan.o Filter.o Sort.o DataRecord.o : Iterator.h
Scan.o : Scan.h
Filter.o : Filter.h
Sort.o : Sort.h
DataRecord.o : DataRecord.h
Tree.o : Tree.h

list : Makefile
	echo Makefile $(HDRS) $(SRCS) $(DOCS) $(SCRS) > list
count : list
	@wc `cat list`

ci :
	ci $(REV) -m"$(MSG)" $(HDRS) $(SRCS) $(DOCS) $(SCRS)
	ci -l $(REV) -m"$(MSG)" Makefile
co :
	co $(REV) -l $(HDRS) $(SRCS) $(DOCS) $(SCRS)

clean :
	@rm -f $(OBJS) Test.exe Test.exe.stackdump trace ExternalSort.exe ExternalSort.exe.stackdump