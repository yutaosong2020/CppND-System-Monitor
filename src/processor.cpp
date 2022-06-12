#include "processor.h"
#include <string>
#include <vector>
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    std::vector<std::string> utilList = LinuxParser::CpuUtilization();
    idle_ = std::stof(utilList[3]);
    total_ = 0;
    for (auto it = utilList.begin(); it != utilList.end(); it++)
    {
        total_ += std::stof(*it);
    }
    return (total_ - idle_) / total_;
}