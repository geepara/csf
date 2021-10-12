#include <iostream>
#include <vector>
#include "Cache.h"
#include "Arguments.h"
#include "cache_sim_fns.h"

int main(int argc, char **argv)
{
  Arguments *arguments = new Arguments();
  readArguments(argc, argv, arguments);
  if (arguments->error == 1)
  {
    fprintf(stderr, "%s", arguments->errorMsg);
    return 1;
  }

  Cache *cache = new Cache(arguments->sets, arguments->blocks, arguments->bytes, arguments->writeAllocate, arguments->writeThrough, arguments->evictions);
  cache->runSim();
  return 0;
}