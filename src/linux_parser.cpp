#include <dirent.h>
#include <unistd.h>
#include <math.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::stoi;
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
    std::istringstream linestream(line); // string stream is seperated by " "
    linestream >> os >> version >> kernel; // this is based on the string info order.
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
        pids.emplace_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  string value;
  float memTotal = 1.0;
  float memFree = 1.0;
  float utilRate = 0.0;
  std::ifstream filestream(kProcDirectory+kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream lineStream(line);
      lineStream >> key >> value;
      if (key == "MemTotal") {
        memTotal = std::stof(value);
      }
      else if (key == "MemFree") {
        memFree = std::stof(value);
      }
    }
  }
  utilRate = (memTotal-memFree)/memTotal;
  return utilRate;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string upTimeStr;
  string idleTimeStr;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line); // string stream is seperated by " "
    linestream >> upTimeStr >> idleTimeStr; // this is based on the string info order.
  }
  long uptimeSec = std::stol(upTimeStr);
  
  return uptimeSec;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  auto cpuValues = CpuUtilization();
  long totalJiffies = 0;
  for(int i=0; (size_t)i<cpuValues.size(); i++)
  {
    totalJiffies += stoi(cpuValues[i]);
  }
  return totalJiffies;
}


// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  return Jiffies() - IdleJiffies();
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  auto cpuValues = CpuUtilization();
  long idleJiffies = stoi(cpuValues[3]);
  return idleJiffies;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  // stream order: user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice
  vector<string> utilisationList {};
  std::ifstream fileStream(kProcDirectory+kStatFilename);
  string line;
  if (fileStream.is_open()) {
    std::getline(fileStream, line);
    std::istringstream lineStream(line);
    int streamIdx = 0;
    std::string curStream;
    while ((lineStream >> curStream) && streamIdx < 11)
    {
      if(streamIdx>0)
      {
        utilisationList.emplace_back(curStream);
      }    
      streamIdx++;
    }
  }
  return utilisationList;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string key;
  string value;
  int totalProcesses = 0;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes") {
        totalProcesses = std::stoi(value);
        break;
      }
    }
  }
  return totalProcesses;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  string value;
  int runningProcesses = 0;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "procs_running") {
        runningProcesses = std::stoi(value);
        break;
      }
    }
  }
  return runningProcesses;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string cmdDir = "/proc/" + to_string(pid) + "/cmdline";
  string cmdLine;
  string line;
  std::ifstream stream(cmdDir);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line); // string stream is seperated by " "
    linestream >> cmdLine; // this is based on the string info order.
  }
  return cmdLine;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) {
  string cmdDir = "/proc/" + to_string(pid) + "/status";
  string line;
  string curKey;
  string value;
  std::ifstream filestream(cmdDir);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> curKey >> value;
      if(curKey == "VmRSS") // "VmSize is the ram size including the virtual ram. Here use the VmRSS physical ram instead."
      {
        int ramUtil = std::stoi(value)/1000;
        return to_string(ramUtil);
      }
    }
  }
  return string();
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string uidDir = "/proc/" + to_string(pid) + "/status";
  return LinuxParser::GetValueFromMap("Uid", uidDir);
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(string uid) {
  // convert uid to user name:
  string line;
  string curUser;
  string curUID;
  string holder;
  std::ifstream filestream(LinuxParser::kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> curUser >> holder >> curUID;
      if(curUID == uid)
      {
        return curUser;
      }
    }
  }
  return curUser;
}
// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string processDir = "/proc/" + to_string(pid) + "/stat";
  std::ifstream stream(processDir);
  string line;
  float startTime = 0.0;
  float hertz = sysconf(_SC_CLK_TCK);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line); // string stream is seperated by " "
    int idx = 0;
    string curStream;
    while ((linestream >> curStream) && idx < 22)
    {
      if(idx == 21)
      {
        startTime = std::stof(curStream);
      }
      idx ++;
    }
  }

  return startTime/hertz;
}

string LinuxParser::GetValueFromMap(string key, string dir)
{
  string line;
  string curKey;
  string value;
  std::ifstream filestream(dir);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> curKey >> value;
      if(curKey == key)
      {
        return value;
      }
    }
  }
  return value;
}

float LinuxParser::CPUUsageRate(int pid)
{
  string processDir = "/proc/" + to_string(pid) + "/stat";
  std::ifstream stream(processDir);
  string line;
  vector<float> processValues = {};
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line); // string stream is seperated by " "
    int idx = 0;
    vector<int> idxList = {13,14,15,16,21};
    string curStream;
    while ((linestream >> curStream) && idx < 22)
    {
      if(std::find(idxList.begin(), idxList.end(), idx) != idxList.end())
      {
        processValues.emplace_back(std::stof(curStream));
      }
      idx ++;
    }
  }
  float total_time = processValues[0] + processValues[1];
  float hertz = sysconf(_SC_CLK_TCK);
  total_time += (processValues[2] + processValues[3]);
  float seconds =  UpTime() - UpTime(pid);
  float cpu_usage = (total_time/hertz)/seconds;
  return cpu_usage;
}
