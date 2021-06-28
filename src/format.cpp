#include "format.h"

#include <string>

using std::string;
using std::to_string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  int ss, mm, hh;
  string time;

  // Compute hours
  hh = seconds / 3600;
  // Compute minutes
  seconds = seconds % 3600;
  mm = seconds / 60;
  // Compute seconds
  ss = seconds % 60;

  if (hh < 10) {
    time = "0";
  }
  time += to_string(hh) + ":";

  if (mm < 10) {
    time += "0";
  }
  time += to_string(mm) + ":";

  if (ss < 10) {
    time += "0";
  }
  time += to_string(ss);

  return time;
}