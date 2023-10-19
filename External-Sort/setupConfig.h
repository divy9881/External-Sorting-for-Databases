#pragma once

#include "defs.h"

#define CACHE_SIZE 1 * 1024 * 1024 // 1 MB
#define DRAM_SIZE 100 * 1024 * 1024 // 100 MB
#define SSD_SIZE 10 * 1024 * 1024 * 1024 // 10 GB
#define SSD_LATENCY 1 / (10 * 1000) // 0.1 ms
#define SSD_BANDWIDTH 100 * 1024 * 1024 // 100 MB/s
#define HDD_SIZE INT_MAX // Infinite
#define HDD_LATENCY 1 * 10 / 1000 // 10 ms
#define HDD_BANDWIDTH 100 * 1024 * 1024 // 100 MB/s

#define RANDOM_INTEGER_RANGE 100