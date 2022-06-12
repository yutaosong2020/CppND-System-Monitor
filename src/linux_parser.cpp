#include <dirent.h>
#include <unistd.h>
#include <math.h>
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
        pids.push_back(pid);
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
      std::istringstream linestream(line);
      linestream >> key >> value;
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
  float uptimeSec = std::stof(upTimeStr);
  
  return uptimeSec;
}

string LinuxParser::SecondToTime (long seconds)
{
  int hours = seconds / 3600;
  int minutes = (seconds % 3600) / 60;
  int secs = (seconds % 3600) % 60;
  
  return to_string(hours) + ":" + to_string(minutes) + ":" + to_string(secs);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> utilisationList {};
  string line;
  string key;
  string user;
  string nice;
  string system;
  string idle;
  string iowait;
  string irq;
  string softirq;
  string steal;
  string guest;
  string guest_nice;
  int totalProcesses = 0;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> key >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
    utilisationList.push_back(user); 
    utilisationList.push_back(nice); 
    utilisationList.push_back(system); 
    utilisationList.push_back(idle); 
    utilisationList.push_back(iowait); 
    utilisationList.push_back(irq); 
    utilisationList.push_back(softirq); 
    utilisationList.push_back(steal); 
    utilisationList.push_back(guest); 
    utilisationList.push_back(guest_nice); 
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
  string cmdDir = "/proc/" + to_string(pid) + "/cmdLine";
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
      if(curKey == "VmSize")
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
        processValues.push_back(std::stof(curStream));
      }
      idx ++;
    }
  }
  float total_time = processValues[0] + processValues[1];
  float hertz = sysconf(_SC_CLK_TCK);
  total_time += (processValues[2] + processValues[3]);
  float seconds =  UpTime(pid);
  float cpu_usage = 100*(total_time/hertz)/seconds;
  return cpu_usage;
}
