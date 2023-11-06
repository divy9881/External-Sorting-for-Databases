#pragma once

#include "DataRecord.h"
#include "defs.h"
#include "setupConfig.h"

/* External Sorting Parameters */
#define OPTIMAL_SSD_PAGE_SIZE (SSD_BANDWIDTH * SSD_LATENCY)
#define OPTIMAL_HDD_PAGE_SIZE (HDD_BANDWIDTH * HDD_LATENCY)
#define OPTIMAL_FAN_IN min((DRAM_SIZE / SSD_PAGE_SIZE), (SSD_SIZE / HDD_PAGE_SIZE))
#define OPTIMAL_NUM_RUNS OPTIMAL_FAN_IN

/* In-Memory Data Records Size */
#define IN_MEMORY_RECORD_SIZE sizeof(DataRecord)
#define TOURNAMENT_TREE_SIZE (OPTIMAL_FAN_IN * IN_MEMORY_RECORD_SIZE)
#define CACHE_SIZE_MINI_RUN ((TOURNAMENT_TREE_SIZE <= CACHE_SIZE) ? 1 : 0)

/* OVC Configurations */
#define OVC_DOMAIN 100