#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "linux_parser.h"
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  Process(int pid);
  void update();
  static bool compare(std::pair<int, Process>& a, std::pair<int, Process>& b);
  // TODO: Declare any necessary private members
 private:
  int pid;
  std::string user;
  std::string command;
  float cpuUtilization;
  std::string ram;
  long int uptime;

  float prev_total_time, prev_seconds;
};

#endif