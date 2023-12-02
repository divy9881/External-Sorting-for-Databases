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
$ valgrind --track-origins=yes --log-file="/External-Sort/valgrind" --leak-check=yes ./ExternalSort.exe
```
