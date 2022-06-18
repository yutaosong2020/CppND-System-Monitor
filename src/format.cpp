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
    string hourStr;
    string minuteStr;
    string secondStr;
    if(hours<10)
    {
        hourStr = "0" + std::to_string(hours);
    }
    else
    {
        hourStr = std::to_string(hours);
    }

    if(minutes<10)
    {
        minuteStr = "0" + std::to_string(minutes);
    }
    else
    {
        minuteStr = std::to_string(minutes);
    }

    if(seconds<10)
    {
        secondStr = "0" + std::to_string(seconds);
    }
    else
    {
        secondStr = std::to_string(seconds);
    }
    

    string time = hourStr + ":" + minuteStr + ":" + secondStr;
    return time;
}