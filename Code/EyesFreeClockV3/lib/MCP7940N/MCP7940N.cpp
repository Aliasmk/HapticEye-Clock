#include "MCP7940N.hpp"
#include "Wire.h"

MCP7940N::MCP7940N(MCP7940N_OSC_Type oscType, bool externalBattery) : oscMode(oscType), extBatt(externalBattery){
    
}

void MCP7940N::initialize(){    
    Serial.println("[RTC]\tBegin read of RTC registers...");
    readAllRTCRegisters();
    Serial.println("[RTC]\tStarting oscillator...");
    startOscillator();
    if(extBatt){
        Serial.println("[RTC]\tEnabling battery backup...");
        enableBackupBattery();
    } else {
        Serial.println("[RTC]\tDisabling battery backup...");
        disableBackupBattery();
    }
    Serial.println("[RTC]\tSetting trim...");
    setTrim(false, 0);
    Serial.println("[RTC]\tInit complete");
}

// TODO give choice for time, alarm, or power down/up registers
void MCP7940N::getTimeAndDate(RTCTime *time){
    readAllRTCRegisters();
    
    //Serial.println(registers[RTCHOUR]);

    time->second =      10 * ((registers[RTCSEC] & SEC_TENS_BITMASK) >> 4) + (registers[RTCSEC] & ONES_BITMASK);
    time->minute =      10 * ((registers[RTCMIN] & MIN_TENS_BITMASK) >> 4) + (registers[RTCMIN] & ONES_BITMASK);
    
    if((registers[RTCHOUR] & B01000000) == B00000000){
        // 24 hour time mode
        Serial.print("[RTC]\t24 hour time detected");
        time->hour =    10 * ((registers[RTCHOUR] & HOUR24_TENS_BITMASK) >> 4) + (registers[RTCHOUR] & ONES_BITMASK);
        time->ampm = TIME_24;
    } else {
        // 12 hour time mode
        time->hour =    10 * ((registers[RTCHOUR] & HOUR12_TENS_BITMASK) >> 4) + (registers[RTCHOUR] & ONES_BITMASK);
        if((registers[RTCHOUR] & B00100000) == B00100000){
            // PM indicator set
            time->ampm = TIME_PM;
        } else {
            time->ampm = TIME_AM;
        }
    }


    
    time->dayOfWeek =   (registers[RTCWKDAY] & DAYOFWEEK_BITMASK);
    time->date =        10 * ((registers[RTCDATE] & DATE_TENS_BITMASK) >> 4) + (registers[RTCDATE] & ONES_BITMASK);
    time->month =       10 * ((registers[RTCMTH] & MNTH_TENS_BITMASK) >> 4) + (registers[RTCMTH] & ONES_BITMASK);
    time->year =        10 * ((registers[RTCYEAR] & YEAR_TENS_BITMASK) >> 4) + (registers[RTCYEAR] & ONES_BITMASK);
}

// TODO give choice for time, alarm, or power down/up registers
void MCP7940N::setTimeAndDate(RTCTime *time){
    // Integer division in all tens calculations!
    registers[RTCSEC] = (((time->second / 10) << 4) & SEC_TENS_BITMASK) + ((time->second % 10) & ONES_BITMASK);
    registers[RTCMIN] = (((time->minute / 10) << 4) & MIN_TENS_BITMASK) + ((time->minute % 10) & ONES_BITMASK);
    if(time->ampm == TIME_24){
        Serial.print("[RTC]\tSetting in 24 hour mode");
        registers[RTCHOUR] = (((time->hour / 10) << 4) & HOUR24_TENS_BITMASK) + ((time->hour % 10) & ONES_BITMASK);
        registers[RTCHOUR] &= B10111111; // clear bit 6 to set to 24 hour mode
        Serial.println(registers[RTCHOUR]);
    } else {
        registers[RTCHOUR] = (((time->hour / 10) << 4) & HOUR12_TENS_BITMASK) + ((time->hour % 10) & ONES_BITMASK);
        if(time->ampm == TIME_AM){
            registers[RTCHOUR] &= B11011111; // clear bit 5 to set to am
        } else {
            registers[RTCHOUR] |= B00100000; // clear bit 5 to set to am
        }
        registers[RTCHOUR] |= B01000000; // set bit 6 to set to 12 hour time
    }

    registers[RTCWKDAY] = (time->dayOfWeek) & DAYOFWEEK_BITMASK;
    registers[RTCDATE] = (((time->date / 10) << 4) & DATE_TENS_BITMASK) + ((time->date % 10) & ONES_BITMASK);
    registers[RTCMTH] = (((time->month / 10) << 4) & MNTH_TENS_BITMASK) + ((time->month % 10) & ONES_BITMASK);
    registers[RTCYEAR] = (((time->year / 10) << 4) & YEAR_TENS_BITMASK) + ((time->year % 10) & ONES_BITMASK);
    stopOscillator();
    writeAllRTCRegisters();
    startOscillator();
}

void MCP7940N::stopOscillator(){
    if(oscMode == EXT_XTAL){
        writeRegister(RTCSEC, 7, 0, 1); // Set ST (RTCSEC BIT7) to 0 to stop oscillator
    } else {
        writeRegister(CONTROL, 3, 0, 1); // Set EXTOSC (CONTROL BIT3) to 0 to stop oscillator
    }
    //Serial.print("[RTC]\tWaiting for oscillator to stop... ");
    while((readRegister(RTCWKDAY) & B00100000) != B00000000){} //wait until oscillator has stopped
    //Serial.println("done");
}

void MCP7940N::startOscillator(){
    if(oscMode == EXT_XTAL){
        writeRegister(RTCSEC, 7, 1, 1); // Set ST (RTCSEC BIT7) to 1 to start oscillator
    } else {
        writeRegister(CONTROL, 3, 1, 1); // Set EXTOSC (CONTROL BIT3) to 1 to start oscillator
    }
    //Serial.print("[RTC]\tRe-enabling oscillator... ");
    while((readRegister(RTCWKDAY) & B00100000) != B00100000){} //wait until oscillator has started
    //Serial.println("done");
}

void MCP7940N::enableBackupBattery(){
    writeRegister(RTCWKDAY, 3, 1, 1); //Set VBATEN (bit 3 of RTCWKDAY) to enable backup battery
}

void MCP7940N::disableBackupBattery(){
    writeRegister(RTCWKDAY, 3, 0, 1); //Clear VBATEN (bit 3 of RTCWKDAY) to disable backup battery
}

bool MCP7940N::isLeapYear(){
    if((readRegister(RTCMTH) & B00100000) == B00100000){
        return true;
    } else {
        return false;
    }
}

void MCP7940N::setTrim(bool addClocks, uint8_t trimValue){
    uint8_t output = 0;
    if(addClocks){
        output |= B10000000;
    }
    output |= (trimValue & B01111111);
    writeRegister(OSCTRIM, 0, output, 7);
}

bool MCP7940N::writeSRAM(uint8_t addr, uint8_t value){
    if(addr < 0x20 || addr > 0x5f){
        return false;
    }
    Wire.beginTransmission(MCP7940N_ADDRESS);
    Wire.write(addr);
    Wire.write(value);
    Wire.endTransmission(true);
    return true;
}

uint8_t MCP7940N::readSRAM(uint8_t addr){
    if(addr < 0x20 || addr > 0x5f){
        return 0;
    }
    
    while(Wire.available()){
        Wire.read();
    }

    Wire.beginTransmission(MCP7940N_ADDRESS);
    Wire.write(addr);
    Wire.endTransmission();
    Wire.requestFrom(MCP7940N_ADDRESS, 1, true);

    uint8_t regValue = Wire.read();
    return regValue;
}

void MCP7940N::writeRegister(MCP7940N_Registers reg, uint8_t offset, uint8_t value, uint8_t len){
    //Serial.print("Writing RTC register ");
    //Serial.print(reg);
    //Serial.print("... ");
    uint8_t bitmask = ((1 << len) - 1) << offset;
    uint8_t valueToWrite = value << offset;
    uint8_t regValue = readRegister(reg);

    //Serial.print("Bitmask: ");
    //Serial.print(bitmask);

    //Serial.print(", value: ");
    //Serial.print(valueToWrite);

    //Serial.print(", was ");
    //Serial.print(regValue);
    regValue = (regValue & ~bitmask) | valueToWrite; 
    registers[reg] = regValue;
    //Serial.print(", now ");
    //Serial.print(regValue);
    Wire.beginTransmission(MCP7940N_ADDRESS);
    Wire.write(reg);
    Wire.write(regValue);
    Wire.endTransmission(true);
    //Serial.println("... Done.");
}

void MCP7940N::writeAllRTCRegisters(){
    while(Wire.available()){
        Wire.read();
    }

    Wire.beginTransmission(MCP7940N_ADDRESS);
    Wire.write(0);
    for(int i = 0; i < MCP7940N_NUM_REGISTERS; i++){
        Wire.write(registers[i]);
    }
    Wire.endTransmission(true);
    
}

uint8_t MCP7940N::readRegister(MCP7940N_Registers reg){
    while(Wire.available()){
        Wire.read();
    }

    Wire.beginTransmission(MCP7940N_ADDRESS);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(MCP7940N_ADDRESS, 1, true);
    uint8_t regValue = Wire.read();

    registers[reg] = regValue;

    return regValue;
}

void MCP7940N::readAllRTCRegisters(){
    while(Wire.available()){
        Wire.read();
    }

    Wire.beginTransmission(MCP7940N_ADDRESS);
    Wire.write(0);
    Wire.endTransmission();

    Wire.requestFrom(MCP7940N_ADDRESS, MCP7940N_NUM_REGISTERS, true);
    int index = 0;
    while(Wire.available()){
        if(index > MCP7940N_NUM_REGISTERS){
            break;
        }
        registers[index++] = Wire.read();
    }
}

