#include "cache_sim_fns.h"
#include "Arguments.h"
#include <iostream>
#include "string.h"

using std::isdigit;
using std::string;

void readArguments(int argc, char **argv, Arguments *arguments)
{
  if (argc != 7)
  {
    arguments->error = 1;
    arguments->errorMsg = ERROR_NUM_ARGS;
  }
  if (!isNumber(argv[1]) || !isNumber(argv[2]) || !isNumber(argv[3]))
  {
    arguments->error = 1;
    arguments->errorMsg = ERROR_INVALID_ARGS;
  }
}

int isNumber(string str)
{
  int len = str.length();
  for (int i = 0; i < len; i++)
  {
    if (!isdigit(str[i]))
    {
      return 0;
    }
  }
  return 1;
}