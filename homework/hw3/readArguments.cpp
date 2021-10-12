#include "readArguments.h"
#include "Arguments.h"
#include <iostream>
#include "string.h"

using std::string;

void readArguments(int argc, char **argv, Arguments *arguments)
{
  if (argc != 7)
  {
    arguments->error = 1;
    arguments->errorMsg = ERROR_NUM_ARGS;
  }
  if (argv[1])
  {
  }
}