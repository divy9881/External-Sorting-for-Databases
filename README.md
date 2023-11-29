# External-Sorting-for-Databases
Implement a External Sorting algorithm for Databases having constrained storage hierarchy


# Initial Setup
```
docker run -it --privileged -v $pwd/External-Sort:/External-Sort ubuntu bash
apt-get update
apt-get install build-essential make g++ vim sudo valgrind -y
cd ./External-Sort
make
./Test.exe
# Run valgrind on Test.exe program
# Creates a log file `valgrind` inside External-Sort directory
valgrind --track-origins=yes --log-file="/External-Sort/valgrind" --leak-check=yes ./Test.exe
```
