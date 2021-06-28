#include "processor.h"

#include <unistd.h>

#include "linux_parser.h"

using std::stof;

// DONE: Return the aggregate CPU utilization   OKEY
float Processor::Utilization() {
  long total_before = LinuxParser::Jiffies();
  long active_before = LinuxParser::ActiveJiffies();

  usleep(100000);  // microseconds --> 100 milliseconds

  float total_after = LinuxParser::Jiffies();
  float active_after = LinuxParser::ActiveJiffies();
  // long total = active_after - active_before;
  // long active = total_after - total_before;
  if (total_after == 0) {
    return 0.0;
  } else {
    return float(active_after) / float(total_after);
  }
}