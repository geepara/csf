#ifndef BLOCK_H
#define BLOCK_H

class Block
{
public:
  unsigned numBytes;
  unsigned tag;
  unsigned address;
  unsigned offset;

  Block(unsigned nBy);
};

#endif //BLOCK_H