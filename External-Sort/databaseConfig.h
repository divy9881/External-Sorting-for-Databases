#pragma once

#include "defs.h"
#include "setupConfig.h"

/* Database column-value Configurations */
#define NUM_CHARS_COL_VALUE 2             // Column-value length
#define DOMAIN_COL_VALUE(a) pow(10, a)    // Domain of Column-value
#define STRING_SIZE 1024

/* Database on-disk record storage size */
/* A DataRecord on disk is represented as Tuple with 2 spaces, 1 newline character, and three column values*/
#define ON_DISK_CHAR_SIZE       1                                     // Space consumed by a character on-disk when persisted on disk
#define ON_DISK_RECORD_SIZE(a)  (2 + 1 + (a * 3)) * ON_DISK_CHAR_SIZE // Space consumed by a record when persisted on disk

/* Database storage format configurations */
#define STORAGE_RECORD_DELIMITER    "|" // Delimiter to separate records from each other
#define STORAGE_COLUMN_DELIMITER    " " // Delimiter to separate column-values from each other

/* Database table format configurations */
#define TABLE_RECORD_DELIMITER "\n" // Delimiter to separate records from each other
#define TABLE_COLUMN_DELIMITER " " // Delimiter to separate column-values from each other

#define MAX_RUNS    100000

