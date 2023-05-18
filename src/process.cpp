#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid)
{
    this->pid = pid;
    string* arr = LinuxParser::CpuUtilizationPerProcess(this->pid);

    if(arr[13].empty() || arr[14].empty() || arr[15].empty() || arr[16].empty() || arr[21].empty())
        return;

    prev_total_time = std::stol(arr[13]) + std::stol(arr[14]) + std::stol(arr[15]) + std::stol(arr[16]);
    prev_seconds = LinuxParser::UpTime() - (std::stol(arr[21]) / sysconf(_SC_CLK_TCK));
}

// TODO: Return this process's ID
int Process::Pid() 
{ 
    return this->pid; 
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() 
{ 
    string* arr = LinuxParser::CpuUtilizationPerProcess(this->pid);

    if(arr[13].empty() || arr[14].empty() || arr[15].empty() || arr[16].empty() || arr[21].empty())
        return 0.0;

    float total_time = std::stol(arr[13]) + std::stol(arr[14]) + std::stol(arr[15]) + std::stol(arr[16]);
    float seconds = LinuxParser::UpTime() - (std::stol(arr[21]) / sysconf(_SC_CLK_TCK));

    if(seconds == prev_seconds)
        return 0.0;

    float cpu_usage = (((total_time - prev_total_time) / (float)sysconf(_SC_CLK_TCK)) / (seconds - prev_seconds));

    prev_total_time = total_time;
    prev_seconds = seconds;

    return cpu_usage;
}

// TODO: Return the command that generated this process
string Process::Command() 
{ 
    return LinuxParser::Command(this->pid);
}

// TODO: Return this process's memory utilization
string Process::Ram() 
{ 
    return LinuxParser::Ram(this->pid);
}

// TODO: Return the user (name) that generated this process
string Process::User() 
{ 
    return LinuxParser::User(this->pid);
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() 
{ 
    return LinuxParser::UpTime() - LinuxParser::UpTime(this->pid);
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const 
{ 
    if(this->cpuUtilization < a.cpuUtilization)
        return true;
    else
        return false;
}

void Process::update()
{
    this->command = this->Command();
    this->ram = this->Ram();
    this->user = this->User();
    this->uptime = this->UpTime();
    this->cpuUtilization = this->CpuUtilization();
}

bool Process::compare(std::pair<int, Process>& a, std::pair<int, Process>& b)
{
    return a.second < b.second;
}