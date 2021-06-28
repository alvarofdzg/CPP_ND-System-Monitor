#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stol;
using std::string;
using std::to_string;
using std::vector;

Process::Process(int a) { Process::pid = a; }

// DONE: Return this process's ID
int Process::Pid() { return pid; }

// DONE: Return this process's CPU utilization   OKEY
float Process::CpuUtilization() {
  long totalTime = LinuxParser::ActiveJiffies(pid);
  long startTime = LinuxParser::UpTime(pid);
  long upTime = LinuxParser::UpTime();

  long seconds = upTime - (startTime / sysconf(_SC_CLK_TCK));

  return (totalTime / sysconf(_SC_CLK_TCK)) / seconds;
}

// DONE: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid); }

// DONE: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid); }

// DONE: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid); }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid); }

// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  return stol(LinuxParser::Ram(pid)) > stol(LinuxParser::Ram(a.pid));
}