#include "processor.h"

#include <unistd.h>

#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::vector;

// DONE: Return the aggregate CPU utilization   OKEY
float Processor::Utilization() {
  vector<string> previous = LinuxParser::CpuUtilization();
  float PrevIdle = stof(previous[LinuxParser::CPUStates::kIdle_]) +
                   stof(previous[LinuxParser::CPUStates::kIOwait_]);

  float PrevNoIdle = stof(previous[LinuxParser::CPUStates::kUser_]) +
                     stof(previous[LinuxParser::CPUStates::kNice_]) +
                     stof(previous[LinuxParser::CPUStates::kSystem_]) +
                     stof(previous[LinuxParser::CPUStates::kIRQ_]) +
                     stof(previous[LinuxParser::CPUStates::kSoftIRQ_]) +
                     stof(previous[LinuxParser::CPUStates::kSteal_]);

  float PrevTotal = PrevIdle + PrevNoIdle;

  usleep(1000000);  // microseconds --> 100 milliseconds

  vector<string> current = LinuxParser::CpuUtilization();
  float CurrentIdle = stof(current[LinuxParser::CPUStates::kIdle_]) +
                      stof(current[LinuxParser::CPUStates::kIOwait_]);

  float CurrentNoIdle = stof(current[LinuxParser::CPUStates::kUser_]) +
                        stof(current[LinuxParser::CPUStates::kNice_]) +
                        stof(current[LinuxParser::CPUStates::kSystem_]) +
                        stof(current[LinuxParser::CPUStates::kIRQ_]) +
                        stof(current[LinuxParser::CPUStates::kSoftIRQ_]) +
                        stof(current[LinuxParser::CPUStates::kSteal_]);

  float CurrentTotal = CurrentIdle + CurrentNoIdle;

  float Total = CurrentTotal - PrevTotal;

  float Idled = CurrentIdle - PrevIdle;

  return (Total - Idled) / Total * 100;
}