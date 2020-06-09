#ifndef TIME_HPP
#define TIME_HPP

#include <MCP7940N.hpp>

#define SRAM_TIMESET_REG 0x20
#define TIME_SET_STAMP 0xA5



class Time{
public:
    Time() : rtc(EXT_XTAL, true) {}

    bool timeSetRequired();
    void setTime(int hour, int minTens, int minOnes, bool pm);
    uint8_t getHours();
    uint8_t getMins();
    uint8_t getMinsTens();
    uint8_t getMinsOnes();
    bool isPM();

private:
    MCP7940N rtc; 
    bool getTime();

    RTCTime time;

};

#endif