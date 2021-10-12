#include "Cache.h"
#include "Set.h"
#include "Block.h"
#include <iostream>
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

  Cache(unsigned s, unsigned b, unsigned by, unsigned wA, unsigned wT, unsigned e)
  {
    this->numSets = s;
    this->numBlocks = b;
    this->numBytes = by;
    this->writeAllocate = wA;
    this->writeThrough = wT;
    this->evictions = e;
    for (int i = 0; i < this->numSets; i++)
    {
      sets.push_back(new Set(this->numBlocks, this->numBytes));
    }
  }

  void runSim()
  {
  }
};