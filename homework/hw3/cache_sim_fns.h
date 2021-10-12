#ifndef CACHE_SIM_FNS_H
#define CACHE_SIM_FNS_H
#include "Arguments.h"
#include <iostream>

#define NO_ERROR "NO_ERROR"
#define ERROR_NUM_ARGS "Incorrect number of arguments"
#define ERROR_INVALID_ARGS "Invalid argument(s)"
#define WRITE_ALLOCATE "write-allocate"
#define NO_WRITE_ALLOCATE "no-write-allocate"
#define WRITE_THROUGH "write-through"
#define WRITE_BACK "write-back"
#define LRU "lru"
#define FIFO "fifo"

void readArguments(int argc, char **argv, Arguments *arguments);

#endif //CACHE_SIM_FNS_H