#ifndef CACHE_H
#define CACHE_H
#include "Set.h"
#include "Block.h"
#include <vector>

using std::vector;

class Cache
{
public:
  unsigned numSets;
  unsigned numBlocks;
  unsigned numBytes;
  unsigned writeAllocate;
  unsigned writeThrough;
  unsigned evictions;
  vector<Set *> sets;

  Cache(unsigned s, unsigned b, unsigned by, unsigned wA, unsigned wT, unsigned e);

  void runSim();
};

#endif // CACHE_H