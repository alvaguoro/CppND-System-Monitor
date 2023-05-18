#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "linux_parser.h"

struct CPU
{
    std::string name;
    long prevIdle, prevIoWait, prevUser, prevNice, prevSystem, prevIrq, prevSoftIrq, prevSteal;
    float utilization = 0;
};

class Processor {
 public:
  void Utilization();  // TODO: See src/processor.cpp
  std::vector<CPU> CPUs;

  Processor();
  // TODO: Declare any necessary private members
 private:
    long prevIdle, prevIoWait, prevUser, prevNice, prevSystem, prevIrq, prevSoftIrq, prevSteal = 0;
};

#endif