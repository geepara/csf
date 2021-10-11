#include <iostream>
#include <vector>
#include "Cache.h"
#include "ArgumentReader.h"

int main(int argc, char **argv)
{
  ArgumentReader *arguments = new ArgumentReader(argc, argv);
  Cache *cache = new Cache(arguments->sets, arguments->blocks, arguments->bytes);
  return 0;
}