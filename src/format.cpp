#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long times) {
    int hours = times/3600;
    int minutes = (times%3600)/60;
    int seconds = ((times%3600)%60);
    string time = std::to_string(hours) + ":" + std::to_string(minutes) + ":" + std::to_string(seconds);
    return time;
}