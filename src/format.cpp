#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) 
{ 
    long H = seconds / 3600;
    long M = (seconds % 3600) / 60;
    long S = (seconds % 3600) % 60;

    std::stringstream strStream;
    strStream << std::setw(2) << std::setfill('0') << H << ":"
                      << std::setw(2) << std::setfill('0') << M << ":" 
                      << std::setw(2) << std::setfill('0') << S;

    return strStream.str(); 
}