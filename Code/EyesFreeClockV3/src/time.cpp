#include <Arduino.h>

#include "time.hpp"

void Time::init(){
    rtc.initialize();
}

bool Time::timeSetRequired(){
    if(rtc.readSRAM(SRAM_TIMESET_REG) != TIME_SET_STAMP){
        return true;
    } else {
        return false;
    }
}

void Time::setTime(int hour, int minTens, int minOnes, bool pm){
    char temp[32];
    snprintf(temp, 32, "[TIME]\tSetting time to %d:%d%d %s", hour, minTens, minOnes, pm == true ?  "pm" : "am");
    Serial.println(temp);

    RTCTime newTime;
    newTime.hour = hour;
    newTime.minute = minTens * 10 + minOnes;
    newTime.second = 0;
    (newTime.ampm == pm) ? TIME_PM : TIME_AM;

    rtc.setTimeAndDate(&newTime);
    rtc.writeSRAM(SRAM_TIMESET_REG, TIME_SET_STAMP);
    Serial.println("[TIME]\t Time set complete");
}

bool Time::getTime(RTCTime *t){
    if(timeSetRequired()){
        return false;
    }
    rtc.getTimeAndDate(t);
    return true;
}


void Time::printTime(){
    char temp[32];
    RTCTime now;
    getTime(&now);
    snprintf(temp, 32, "Current time: %d:%02d:%02d %s", now.hour, now.minute, now.second, now.ampm == TIME_PM ?  "pm" : "am");
    Serial.println(temp);
}