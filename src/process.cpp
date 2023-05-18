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
    
    prev_total_time = LinuxParser::ActiveJiffies(pid);
    prev_seconds = LinuxParser::UpTime() - LinuxParser::UpTime(pid);
}

// TODO: Return this process's ID
int Process::Pid() 
{ 
    return this->pid; 
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() 
{ 
    float total_time = LinuxParser::ActiveJiffies(this->pid);
    float seconds = LinuxParser::UpTime() - LinuxParser::UpTime(this->pid);

    if(seconds == prev_seconds)
        return 0.0;

    float cpu_usage = (((total_time - prev_total_time) / (float)sysconf(_SC_CLK_TCK)) / (seconds - prev_seconds));

    if(total_time != prev_total_time)
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