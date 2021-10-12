#ifndef TRACE_H
#define TRACE_H

class Trace
{
public:
  unsigned instruction;
  unsigned address;

  Trace(unsigned i, unsigned a);
};

#endif //TRACE_H