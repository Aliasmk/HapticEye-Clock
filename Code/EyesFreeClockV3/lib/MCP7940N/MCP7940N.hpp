#ifndef MCP7940N_H
#define MCP7940N_H

#define MCP7940N_ADDRESS 0x6F


#define ONES_BITMASK B00001111
#define SEC_TENS_BITMASK B01110000
#define MIN_TENS_BITMASK B01110000
#define HOUR24_TENS_BITMASK B00110000
#define HOUR12_TENS_BITMASK B00010000
#define DAYOFWEEK_BITMASK B00000111
#define DATE_TENS_BITMASK B00110000
#define MNTH_TENS_BITMASK B00010000
#define YEAR_TENS_BITMASK B11110000

#include <stdint.h>

// TODO: Alarms
// TODO: Getting and setting power-fail times
// TODO: Output Configurations

#include "RTCTime.hpp"


typedef enum MCP7940N_Registers {
    RTCSEC,
    RTCMIN,
    RTCHOUR,
    RTCWKDAY,
    RTCDATE,
    RTCMTH,
    RTCYEAR,
    CONTROL,
    OSCTRIM,
    RESERVED_09h,
    ALM0SEC,
    ALM0MIN,
    ALM0HOUR,
    ALM0WKDAY,
    ALM0DATE,
    ALM0MTH,
    RESERVED_10h,
    ALM1SEC,
    ALM1MIN,
    ALM1HOUR,
    ALM1WKDAY,
    ALM1DATE,
    ALM1MTH,
    RESERVED_17h,
    PWRDNMIN,
    PWRDNHOUR,
    PWRDNDATE,
    PWRDNMTH,
    PWRUPMIN,
    PWRUPHOUR,
    PWRUPDATE,
    PWRUPMTH,
    MCP7940N_NUM_REGISTERS
} MCP7940N_Registers;

typedef enum MCP7940N_OSC_Type {
    EXT_XTAL,
    EXT_CLK
} MCP7940N_OSC_Type;


class MCP7940N {
public:
    MCP7940N(MCP7940N_OSC_Type oscType, bool externalBattery);
    void initialize();

    void getTimeAndDate(RTCTime *time);
    void setTimeAndDate(RTCTime *time);
    bool isLeapYear();

    void setTrim(bool addClocks, uint8_t trimval);

    bool writeSRAM(uint8_t addr, uint8_t value);
    uint8_t readSRAM(uint8_t addr);

private:
    uint8_t registers[MCP7940N_NUM_REGISTERS];
    void writeRegister(MCP7940N_Registers reg, uint8_t offset, uint8_t value, uint8_t length);
    uint8_t readRegister(MCP7940N_Registers reg);
    void readAllRTCRegisters();
    void writeAllRTCRegisters();

    void stopOscillator();
    void startOscillator();

    void enableBackupBattery();
    void disableBackupBattery();

    MCP7940N_OSC_Type oscMode;
    bool extBatt;
};


#endif