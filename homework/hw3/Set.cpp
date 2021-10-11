#include "Set.h"
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

  Set(unsigned nB, unsigned nBy)
  {
    this->numBlocks = nB;
    this->numBytes = nBy;
    for (int i = 0; i < this->numBlocks; i++)
    {
      blocks.push_back(new Block(this->numBytes));
    }
  }
};