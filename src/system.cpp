#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric criteria "The student will be able to extract and display basic data about the system."

You need to properly format the uptime. Refer to the comments mentioned in format. cpp for formatting the uptime.*/

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
    std::vector<int> pids_ = LinuxParser::Pids();
    
    // initiate all processes object:
    for(int j=0; j<pids_.size(); j++)
    {
        int curPID = pids_[j];
        Process curProcess;
        curProcess.Pid(curPID);
        processes_.push_back(curProcess);
    }

    // sort objects:


    return processes_;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() {
    kernel_ = LinuxParser::Kernel();
    return kernel_;
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() {
    float memoryUtilization_ = LinuxParser::MemoryUtilization();
    return memoryUtilization_;
}

// TODO: Return the operating system name
std::string System::OperatingSystem() {
    operatingSystem_ = LinuxParser::OperatingSystem();

    return operatingSystem_; 
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() {
    runningProcesses_ = LinuxParser::RunningProcesses();
    return runningProcesses_;
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() {
    totalProcesses_ = LinuxParser::TotalProcesses();
    return totalProcesses_;
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime() {
    upTime_ = LinuxParser::UpTime();
    return upTime_;
}
