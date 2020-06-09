#ifndef TIME_HPP
#define TIME_HPP

#include <MCP7940N.hpp>

class Time{
public:
    Time() : rtc(EXT_XTAL, true) {}
private:
    MCP7940N rtc; 

};

#endif