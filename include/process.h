#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
public:
    Process();
    Process(int pid);
    int Pid();
    void Pid(int pid);                               // TODO: See src/process.cpp
    std::string User();                      // TODO: See src/process.cpp
    std::string Command();                   // TODO: See src/process.cpp
    float CpuUtilization();                  // TODO: See src/process.cpp
    std::string Ram();                       // TODO: See src/process.cpp
    long int UpTime();                       // TODO: See src/process.cpp
    bool operator<(Process const& a) const;  // TODO: See src/process.cpp
    bool IsValid();

  // TODO: Declare any necessary private members
private:
    int pid_;
    std::string user_;
    std::string command_;
    float cpuUtil_;
    std::string ram_;
    long int upTime_;
};

#endif