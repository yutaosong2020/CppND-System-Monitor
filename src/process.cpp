#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() {

    return pid_;
}
void Process::Pid(int pid) {
    pid_ = pid;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
    cpuUtil_ = LinuxParser::CPUUsageRate(pid_);
    return cpuUtil_;
}


// TODO: Return the command that generated this process
string Process::Command() {
    command_ = LinuxParser::Command(pid_);
    return command_;
}

// TODO: Return this process's memory utilization
string Process::Ram() { return string(); }

// TODO: Return the user (name) that generated this process
string Process::User() {
    string uid = LinuxParser::Uid(pid_);
    user_ = LinuxParser::User(uid);
    return user_;
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
    upTime_ = LinuxParser::UpTime(pid_);
    return upTime_;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
    if(this->cpuUtil_ < a.cpuUtil_)
    {
        return true;
    }
    else
    {
        return false;
    }  
}