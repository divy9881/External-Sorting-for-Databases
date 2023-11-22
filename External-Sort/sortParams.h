#pragma once

#include "DataRecord.h"
#include "defs.h"
#include "setupConfig.h"

/* External Sorting Parameters */
#define OPTIMAL_SSD_PAGE_SIZE   (SSD_BANDWIDTH * SSD_LATENCY)               // Optimal page-size for SSD device
#define OPTIMAL_HDD_PAGE_SIZE   (HDD_BANDWIDTH * HDD_LATENCY)               // Optimal page-size for HDD device
#define OPTIMAL_FAN_IN          min((DRAM_SIZE / OPTIMAL_SSD_PAGE_SIZE), ((lluint)SSD_SIZE / OPTIMAL_HDD_PAGE_SIZE)) // Optimal merge fan-in
#define OPTIMAL_NUM_RUNS        OPTIMAL_FAN_IN                              // Optimal number of runs to be merged by a tournament tree in a single pass
#define MAX_SSD_NUM_RUNS        ((lluint)SSD_SIZE / OPTIMAL_HDD_PAGE_SIZE)  // Maximum possible number of runs on SSD give the run-size storage
#define MAX_DRAM_NUM_RUNS       (DRAM_SIZE / OPTIMAL_SSD_PAGE_SIZE)         // Maximum possible number of runs on HDD give the run-size storage

/* In-Memory Data Records Size */
#define IN_MEMORY_RECORD_SIZE   sizeof(DataRecord)
#define TOURNAMENT_TREE_SIZE    (OPTIMAL_FAN_IN * IN_MEMORY_RECORD_SIZE)
#define CACHE_SIZE_MINI_RUN     ((TOURNAMENT_TREE_SIZE <= CACHE_SIZE) ? 1 : 0)

/* OVC Configurations */
#define OVC_DOMAIN 100