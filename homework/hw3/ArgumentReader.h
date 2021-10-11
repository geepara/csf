#ifndef ARGUMENT_READER_H
#define ARGUMENT_READER_H
#include <iostream>

class ArgumentReader
{
public:
  unsigned sets;
  unsigned blocks;
  unsigned bytes;

  ArgumentReader(int argc, char **argv);
};

#endif