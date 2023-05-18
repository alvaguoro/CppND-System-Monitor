#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() 
{ 
  string discard_name;
  float discard_value;
  float memTotal;
  float memActive;

  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  string line;
  if (stream.is_open()) 
  {
    while(std::getline(stream, line))
    {
      std::istringstream linestream(line);
      linestream >> discard_name >> discard_value;

      if(discard_name == "MemTotal:")
        memTotal = discard_value;

      if(discard_name == "Active:")
      {
        memActive = discard_value;
        break;
      }
    }
  }

  return memActive / memTotal;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() 
{
  long uptime = 0;

  std::ifstream stream(kProcDirectory + kUptimeFilename);
  string line;
  if (stream.is_open() && std::getline(stream, line)) 
  {
    std::istringstream linestream(line);
    linestream >> uptime;
  }

  return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() 
{ 
  //Already done with CpuUtilization
  return 0;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) 
{ 
  string line, time;
  long activeJiffies = 0;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);

  if (stream.is_open()) 
  {
    if(std::getline(stream, line))
    {
      std::istringstream linestream(line);
      int i = 0;
      while (linestream >> time) 
      {
        if(i == 13 || i == 14 || i == 15 || i == 16)
          activeJiffies += stol(time);

        ++i;
      }
    }
    else
    {
      return 0;
    }
  }

  return activeJiffies;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() 
{ 
  //Already done with CpuUtilization
  return 0;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() 
{ 
  //Already done with CpuUtilization
  return 0;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() 
{ 
  vector<string> res;
  std::ifstream stream(kProcDirectory + kStatFilename);
  string line;
  if (stream.is_open()) 
  {
    while(std::getline(stream, line))
    {
      if(line.find("cpu") != string::npos)
        res.push_back(line);
    }  
  }

  return res;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() 
{ 
  string discard_name;
  long discard_value;

  std::ifstream stream(kProcDirectory + kStatFilename);
  string line;
  if (stream.is_open()) 
  {
    while(std::getline(stream, line))
    {
      std::istringstream linestream(line);
      linestream >> discard_name >> discard_value;

      if(discard_name == "processes")
        break;
    }
  }

  return discard_value;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() 
{ 
  string discard_name;
  long discard_value;

  std::ifstream stream(kProcDirectory + kStatFilename);
  string line;
  if (stream.is_open()) 
  {
    while(std::getline(stream, line))
    {
      std::istringstream linestream(line);
      linestream >> discard_name >> discard_value;

      if(discard_name == "procs_running")
        break;
    }
  }

  return discard_value;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) 
{ 
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  string line;
  if (stream.is_open()) 
  {
    std::getline(stream, line);    
  }

  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) 
{ 
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  string line, discardName;
  int ram = 0;
  if (stream.is_open()) 
  {
    while(std::getline(stream, line))
    {
      std::istringstream linestream(line);
      linestream >> discardName >> ram;
      
      if(discardName == "VmSize:")
        break;
    }
  }

  return to_string(ram/1024);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) 
{ 
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  string line, discardName, uid;
  if (stream.is_open()) 
  {
    while(std::getline(stream, line))
    {
      std::istringstream linestream(line);
      linestream >> discardName >> uid;
      
      if(discardName == "Uid:")
        break;
    }
  }

  return uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) 
{ 
  string uid = LinuxParser::Uid(pid);

  std::ifstream stream(kPasswordPath);
  string line, username, uidDiscard;

  if (stream.is_open()) 
  {
    while(std::getline(stream, line))
    {
      std::istringstream linestream(line);
      string token;
      string tokens[3];

      for(int i = 0; i < 3; i++)
      {
        std::getline(linestream, token, ':');
        tokens[i] = token;
      }
      
      username = tokens[0];
      uidDiscard = tokens[2];
        
      if(uidDiscard == uid)
        break;
      
    }
  }

  return username;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) 
{  
  string uid = LinuxParser::Uid(pid);

  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  string line;
  string arr[22];
  if (stream.is_open()) 
  {
    int i = 0;
    if(std::getline(stream, line))
    {
      std::istringstream linestream(line);
      while(linestream.good() && i < 22)
      {
        linestream >> arr[i];
        ++i;
      }
    }
  }

  if(arr[21].empty())
    return 0;

  return std::stol(arr[21])/sysconf(_SC_CLK_TCK);
}
