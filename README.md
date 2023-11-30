docker run -it --privileged -v $pwd/External-Sort:/External-Sort ubuntu bash
apt-get update
apt-get install build-essential make g++ vim sudo valgrind -y
cd ./External-Sort
make
./Test.exe

# Run valgrind on Test.exe program
# Creates a log file `valgrind` inside External-Sort directory
valgrind --track-origins=yes --log-file="/External-Sort/valgrind" --leak-check=yes ./Test.exe

# To generate ExternalSort.exe
# Run following make command
make ExternalSort.exe

# Where,
# "-c" gives the total number of records
# "-s" is the individual record size
# "-o" is the trace of your program run
./ExternalSort.exe -c 120 -s 1000 -o trace0.txt
