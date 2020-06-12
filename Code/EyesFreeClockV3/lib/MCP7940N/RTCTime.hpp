#ifndef RTCTIME_HPP
#define RTCTIME_HPP

#include <stdio.h>

typedef enum AMPMType{
    TIME_AM,
    TIME_PM,
    TIME_24
} AMPMType;

typedef struct RTCTime {
    uint8_t year;
    uint8_t month;
    uint8_t date;
    uint8_t dayOfWeek;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    AMPMType ampm;
} RTCTime;

#endif