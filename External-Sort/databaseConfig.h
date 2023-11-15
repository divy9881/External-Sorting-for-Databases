#pragma once

#include "defs.h"
#include "setupConfig.h"

/* Database Configurations */
#define NUM_CHARS_COL_VALUE 2
#define DOMAIN_COL_VALUE pow(10, NUM_CHARS_COL_VALUE)
#define ON_DISK_CHAR_SIZE 1
/* A DataRecord on disk is represented as Tuple with 2 spaces, 1 newline character, and three column values*/
#define ON_DISK_RECORD_SIZE (2 + 1 + (NUM_CHARS_COL_VALUE * 3)) * ON_DISK_CHAR_SIZE
#define RECORD_DELIMITER "|"
#define COLUMN_DELIMITER " "

