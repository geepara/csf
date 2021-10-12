#ifndef READ_ARGUMENTS_H
#define READ_ARGUMENTS_H
#include "Arguments.h"
#include <iostream>

#define NO_ERROR "NO_ERROR"
#define ERROR_NUM_ARGS "Incorrect number of arguments"
#define ERROR_INVALID_ARGS "Invalid argument(s)"

void readArguments(int argc, char **argv, Arguments *arguments);

#endif //READ_ARGUMENTS_H