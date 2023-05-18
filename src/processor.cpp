#include "processor.h"
#include <iostream>

using std::string;
using std::vector;

Processor::Processor()
{
    vector<string> cpus1 = LinuxParser::CpuUtilization();
    for(int i = 0; i < (int)cpus1.size(); i++)
    {
        std::istringstream linestream1(cpus1[i]);
        CPU cpu;
        linestream1 >> cpu.name >> cpu.prevUser >> cpu.prevNice >> cpu.prevSystem >> cpu.prevIdle >> cpu.prevIoWait >> cpu.prevIrq >> cpu.prevSoftIrq >> cpu.prevSteal;
        transform(cpu.name.begin(), cpu.name.end(), cpu.name.begin(), ::toupper);
        CPUs.push_back(cpu);
    }
}

// TODO: Return the aggregate CPU utilization
// Used this algorithm https://stackoverflow.com/a/23376195 
void Processor::Utilization() 
{ 
    vector<string> cpus2 = LinuxParser::CpuUtilization();

    string discard;

    for(int i = 0; i < (int)cpus2.size(); i++)
    {
        std::istringstream linestream2(cpus2[i]);

        long idle, ioWait, user, nice, system, irq, softirq, steal;
             
        long PrevIdle, Idle, PrevNonIdle, NonIdle, PrevTotal, Total;

        linestream2 >> discard >> user >> nice >> system >> idle >> ioWait >> irq >> softirq >> steal;

        PrevIdle = CPUs[i].prevIdle + CPUs[i].prevIoWait;
        Idle = idle + ioWait;

        PrevNonIdle = CPUs[i].prevUser + CPUs[i].prevNice + CPUs[i].prevSystem + CPUs[i].prevIrq + CPUs[i].prevSoftIrq + CPUs[i].prevSteal;
        NonIdle = user + nice + system + irq + softirq + steal;

        PrevTotal = PrevIdle + PrevNonIdle;
        Total = Idle + NonIdle;

        long totald = Total - PrevTotal;
        long idled = Idle - PrevIdle;

        float utilization = totald == 0 ? 0 : (totald - idled)/(float)totald;

        CPUs[i].utilization = utilization;
        CPUs[i].prevUser = user;
        CPUs[i].prevNice = nice;
        CPUs[i].prevSystem = system;
        CPUs[i].prevIdle = idle;
        CPUs[i].prevIoWait = ioWait;
        CPUs[i].prevIrq = irq;
        CPUs[i].prevSoftIrq = softirq;
        CPUs[i].prevSteal = steal;
    }
}