#pragma once

#include "defs.h"
#include "setupConfig.h"

/* Database column-value Configurations */
#define NUM_CHARS_COL_VALUE 2                               // Column-value length
#define DOMAIN_COL_VALUE    pow(10, NUM_CHARS_COL_VALUE)    // Domain of Column-value

/* Database on-disk record storage size */
/* A DataRecord on disk is represented as Tuple with 2 spaces, 1 newline character, and three column values*/
#define ON_DISK_CHAR_SIZE   1                                                       // Space consumed by a character on-disk when persisted on disk
#define ON_DISK_RECORD_SIZE (2 + 1 + (NUM_CHARS_COL_VALUE * 3)) * ON_DISK_CHAR_SIZE // Space consumed by a record when persisted on disk

/* Database storage format configurations */
#define RECORD_DELIMITER    "|" // Delimiter to separate records from each other
#define COLUMN_DELIMITER    " " // Delimiter to separate column-values from each other

#define MAX_RUNS    100000

