#ifndef ARGUMENTS_H
#define ARGUMENTS_H
#include <iostream>
#include "string.h"

using std::string;

class Arguments
{
public:
  unsigned sets;
  unsigned blocks;
  unsigned bytes;
  unsigned error;
  unsigned writeAllocate;
  unsigned writeThrough;
  unsigned evictions;
  string errorMsg;
};

#endif //ARGUMENTS_H