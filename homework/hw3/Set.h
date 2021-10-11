#ifndef SET_H
#define SET_H
#include "Block.h"
#include <iostream>
#include <vector>

using std::vector;

class Set
{
public:
  unsigned numBlocks;
  unsigned numBytes;
  vector<Block *> blocks;

  Set(unsigned nB, unsigned nBy);
};

#endif //SET_H