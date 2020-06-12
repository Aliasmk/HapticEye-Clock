#ifndef TIME_HPP
#define TIME_HPP

#include <MCP7940N.hpp>

#define SRAM_TIMESET_REG 0x20
#define TIME_SET_STAMP 0xA5



class Time{
public:
    Time() : rtc(EXT_XTAL, true) {}

    void init();

    bool timeSetRequired();
    void setTime(int hour, int minTens, int minOnes, bool pm);
    bool getTime(RTCTime *time);

    void printTime();

private:
    MCP7940N rtc; 
};

#endif