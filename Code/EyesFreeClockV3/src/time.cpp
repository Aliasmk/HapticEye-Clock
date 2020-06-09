#include <Arduino.h>

#include "time.hpp"

struct FormattedTime {
    uint8_t hour;
    uint8_t min;
    bool pm;
} FormattedTime;

bool Time::timeSetRequired(){
    if(rtc.readSRAM(SRAM_TIMESET_REG) != TIME_SET_STAMP){
        return true;
    } else {
        return false;
    }
}

void Time::setTime(int hour, int minTens, int minOnes, bool pm){
    Serial.print("[TIME]\tSetting time to ");
    Serial.print(hour);
    Serial.print(":");
    Serial.print(minTens);
    Serial.print(minOnes);
    Serial.println(pm ? " pm" : " am");

    RTCTime newTime;
    newTime.hour = hour;
    newTime.minute = minTens * 10 + minOnes;
    newTime.second = 0;
    newTime.ampm = pm ? TIME_PM : TIME_AM;

    rtc.setTimeAndDate(&newTime);
    rtc.writeSRAM(SRAM_TIMESET_REG, TIME_SET_STAMP);
    Serial.println("[TIME]\t Time set complete");
}

bool Time::getTime(){
    if(timeSetRequired()){
        return false;
    }

    rtc.getTimeAndDate(&time);
    return true;
}

uint8_t Time::getHours(){
    if(getTime()){
        return time.hour;
    }
    return 0;
}

uint8_t Time::getMins(){
    if(getTime()){
        return time.minute;
    }
    return 0;
}

bool Time::isPM(){
    if(getTime()){
        if(time.ampm == TIME_PM){
            return true;
        } else {
            return false;
        }
    }
    return false; //welp
    
}