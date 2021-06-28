#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::stoi;
using std::stol;
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

// DONE: Read and return the system memory utilization   OKEY
float LinuxParser::MemoryUtilization() {
  float total, free;
  string line, key, value;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "MemTotal:") {
        total = stof(value);
      } else if (key == "MemFree:") {
        free = stof(value);
      }
    }
    return (total - free) / total;
  }
  return 0.0;
}

// DONE: Read and return the system uptime   OKEY
long LinuxParser::UpTime() {
  string line, value;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value;
    return stol(value);
  }
  return 0;
}

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  long int jiffies = 0;
  vector<string> values = LinuxParser::CpuUtilization();
  for (int i = kUser_; i <= kSteal_; i++) {
    jiffies += stol(values[i]);
  }
  return jiffies;
}

// DONE: Read and return the number of active jiffies for a PID   OKEY
long LinuxParser::ActiveJiffies(int pid) {
  string line, value;
  long jiffies = 0;
  std::ifstream stream(kProcDirectory + "/" + to_string(pid) + "/" +
                       kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = kPid_; i <= kCStime_; i++) {
      linestream >> value;
      if (i == kUtime_ || i == kStime_ || i == kCUtime_ || i == kCStime_) {
        jiffies += stol(value);
      }
    }
  }
  return jiffies;
}

// DONE: Read and return the number of active jiffies for the system   OKEY
long LinuxParser::ActiveJiffies() {
  return LinuxParser::Jiffies() - LinuxParser::IdleJiffies();
}

// DONE: Read and return the number of idle jiffies for the system   OKEY
long LinuxParser::IdleJiffies() {
  vector<string> cpu = LinuxParser::CpuUtilization();
  long idle = 0;
  idle += stol(cpu[kIdle_]);
  idle += stol(cpu[kIOwait_]);
  return idle;
}

// DONE: Read and return CPU utilization   OKEY
vector<string> LinuxParser::CpuUtilization() {
  vector<string> values;
  string line, key;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> key) {
      if (key != "cpu") {
        values.push_back(key);
      }
    }
  }
  return values;
}

// DONE: Read and return the total number of processes   OKEY
int LinuxParser::TotalProcesses() {
  int processes;
  string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes") {
        processes = stoi(value);
      }
    }
  }
  return processes;
}

// DONE: Read and return the number of running processes   OKEY
int LinuxParser::RunningProcesses() {
  int processes;
  string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "procs_running") {
        processes = stoi(value);
      }
    }
  }
  return processes;
}

// DONE: Read and return the command associated with a process   OKEY
string LinuxParser::Command(int pid) {
  string command;
  std::ifstream stream(kProcDirectory + "/" + to_string(pid) + "/" +
                       kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, command);
  }
  return command;
}

// DONE: Read and return the memory used by a process   OKEY
string LinuxParser::Ram(int pid) {
  string line, key, ram;
  std::stringstream ram2;
  std::ifstream stream(kProcDirectory + "/" + to_string(pid) + "/" +
                       kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> ram;
      if (key == "VmSize:") {
        // return to_string(ram/1000);
        ram2 << std::fixed << std::setprecision(3) << stof(ram) / 1000;
        return ram2.str();
      }
    }
  }
  return string();
}

// DONE: Read and return the user ID associated with a process   OKEY
string LinuxParser::Uid(int pid) {
  string line, key, uid;
  std::ifstream stream(kProcDirectory + "/" + to_string(pid) + "/" +
                       kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> uid;
      if (key == "Uid:") {
        return uid;
      }
    }
  }
  return string();
}

// DONE: Read and return the user associated with a process   OKEY
string LinuxParser::User(int pid) {
  string uid = LinuxParser::Uid(pid);
  string line, value, key, x;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> x >> value;
      if (value == uid) {
        return key;
      }
    }
  }
  return string();
}

// DONE: Read and return the uptime of a process   OKEY
long LinuxParser::UpTime(int pid) {
  string line, value;
  std::ifstream stream(kProcDirectory + "/" + to_string(pid) + "/" +
                       kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    int a = 0;
    while (linestream >> value) {
      if (a == 21) {
        return stol(value);  // sysconf(_SC_CLK_TCK);
        // Una vez que alcanzamos el valor de la posición 21 (el que queremos)
        // nos indica la cantidad de tics de reloj que suceden, por lo que
        // devemos dividirlo entre sysconf(_SC_CLK_TCK) para pasarlo a segundos.
      }
      a++;
    }
  }
  return 0;
}