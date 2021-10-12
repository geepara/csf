#include "Trace.h"
#include <iostream>

class Trace
{
public:
  unsigned instruction;
  unsigned address;

  Trace(unsigned i, unsigned a)
  {
    this->instruction = i;
    this->address = a;
  }
};