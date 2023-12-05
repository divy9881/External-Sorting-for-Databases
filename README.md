# External-Sorting-for-Databases
External Sorting algorithm for Databases having constrained storage hierarchy

# Group Members
- Divy Patel (9085310937) dspatel6@wisc.edu
- Sahil Naphade (9085746619) smnaphade@wisc.edu
- Devaki Kulkarni (9086222321) dgkulkarni2@wisc.edu
- Manaswini Gogineni (9085432699) mgogineni@wisc.edu 

# Techniques Implemented by our submission and the corresponding Source Files and Lines

- Tournament trees [5]
- Offset-value coding [5]
- Minimum count of row & column comparisons [5]
- Cache-size mini runs [5]
- Device-optimized page sizes [5]
- Spilling memory-to-SSD [5]
- Spilling from SSD to disk [5]
- Graceful degradation
  - Into merging [5]
  - Beyond one merge step [5]
- Optimized merge patterns [5]
- Verifying
  - sets of rows & values [5]
  - sort order [5]

- Replacement selection?
- Run size > memory size?
- Variable-size records?
- Compression?
- Prefix truncation?
- Quicksort


# Reasons we chose to implement the specific subset of techniques 
# Project's state(complete or have what kinds of bugs) 
# How to run our programs

# Initial Setup
```
$ docker run -it --privileged -v $pwd/External-Sort:/External-Sort ubuntu bash
$ apt-get update
$ apt-get install build-essential make g++ vim sudo valgrind -y
$ cd ./External-Sort
```

# Generating ExternalSort.exe and Running the Executable
```
$ make ExternalSort.exe

# Where,
# "-c" gives the total number of records
# "-s" is the individual record size
# "-o" is the trace of your program run
$ ./ExternalSort.exe -c 120 -s 1000 -o trace0.txt
```

# Run Valgrind for a Leak Check
```
# Creates a log file `valgrind` inside the External-Sort directory
$ valgrind --track-origins=yes --log-file="/External-Sort/valgrind" --leak-check=yes ./ExternalSort.exe -c 120 -s 1000 -o trace0.txt
```
