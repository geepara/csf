#include "Block.h"
#include <iostream>

class Block
{
public:
  unsigned numBytes;
  unsigned tag;
  unsigned address;
  unsigned offset;

  Block(unsigned nBy)
  {
    this->numBytes = nBy;
  }
};