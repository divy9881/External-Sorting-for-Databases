# External-Sorting-for-Databases
Implement a External Sorting algorithm for Databases having constrained storage hierarchy


# Initial Setup
```
docker run -it -v $pwd/External-Sort:/External-Sort ubuntu bash
apt-get update
apt-get install build-essential -y
apt-get install make -y
apt-get install g++ -y
```

## Data format:
All records will have 3 columns. 
For initial testing, we can use 3 random ints; for 1KB records, have 3 random 333byte columns
Extra credits for random data length.

## Memory/Persistent storage limits
1. 1M Cache
2. 100M DRAM
3. 10G SSD
4. Unlimited HDD storage
