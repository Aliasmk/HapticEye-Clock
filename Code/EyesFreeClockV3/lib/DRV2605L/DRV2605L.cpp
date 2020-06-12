#include "DRV2605L.hpp"

#include <Wire.h>

DRV2605L::DRV2605L(MotorType type, OperationMode mode, int enablePin) : type_(type), mode_(mode), enablePin_(enablePin){
    Wire.begin();  
    pinMode(enablePin_, OUTPUT);
}

void DRV2605L::initialize(){
    enable();
    writeRegister(MODE, 0, 0, 8);
    writeRegister(FEEDBACK_CTRL, N_ERM_LRA_OFFSET, 0, 1);
    writeRegister(CONTROL3, 5, 1, 1); //open loop erm bit
    writeRegister(OD_CLAMP_V, 0, 0xFF,8);
    setTriggerType(TRIG_INTERNAL);

    /*if(mode_ == OPEN_LOOP){
        // Set up for Open Loop mode
        if(type_ == MOTOR_ERM){
            // Set up an ERM Motor
            writeRegister(FEEDBACK_CTRL, N_ERM_LRA_OFFSET, 0, 1);
            writeRegister(CONTROL3, 5, 1, 1); //open loop erm bit
            setTriggerType(TRIG_INTERNAL);
        } else {
            // Set up an LRA Motor


        }

    } else {
        // Set up for Closed Loop mode
        if(type_ == MOTOR_ERM){
         //Set up an ERM Motor
            enable();
            writeRegister(FEEDBACK_CTRL, N_ERM_LRA_OFFSET, 0, 1);
            writeRegister(MODE, 0, 0x7, 8);
            autoCalibrate();
            writeRegister(GO,0,1,1);
            getRegisterData(GO);
            while((registers[GO]&0x1) != 0) {
                getRegisterData(GO);
            }
            setTriggerType(TRIG_INTERNAL);
        } else {
            // Set up an LRA Motor


        }

    }*/
}

void DRV2605L::enable(){
    digitalWrite(enablePin_, HIGH);
    enabled = true;
}

void DRV2605L::disable(){
    digitalWrite(enablePin_, LOW);
    enabled = false;
}

void DRV2605L::setTriggerType(TriggerType trig){
    writeRegister(MODE, MODE_OFFSET, trig, 3);
}

void DRV2605L::setWaveformLibrary(WaveformLibrary lib){
    writeRegister(LIB_SEL, 0, lib, 3);
}

void DRV2605L::queueWaveform(int index, int data, bool wait){
    if(index > 7 || index < 0){
        return;
    }
    if(wait){
        writeRegister(0x4+index, 7, 1, 1); //write WAIT bit
    }
    writeRegister(0x4+index, 0, data, 7);
}

void DRV2605L::playWaveforms(){
    writeRegister(GO, 0, 1, 1);
}

void DRV2605L::stopWaveforms(){
    writeRegister(GO, 0, 0, 1);
}

void DRV2605L::autoCalibrate(){
    writeRegister(FEEDBACK_CTRL, N_ERM_LRA_OFFSET, 0x0, 1);
    writeRegister(FEEDBACK_CTRL, FB_BRAKE_FACTOR_OFFSET, 0x2, 3);
    writeRegister(FEEDBACK_CTRL, LOOP_GAIN_OFFSET, 0x2, 2);
    writeRegister(RATED_V, RATED_VOLTAGE_OFFSET, 0xEC, 8);
    //writeRegister(OD_CLAMP_V, OD_CLAMP_OFFSET);
    writeRegister(CONTROL4, AUTO_CAL_TIME_OFFSET, 0x03, 2);
    writeRegister(CONTROL1, 0, 0x13, 5); //sample time
    writeRegister(CONTROL2, 2, 0x1, 2);  //blanking time
    writeRegister(CONTROL2, 0, 0x1, 2); // i diss time
    writeRegister(CONTROL5, 6, 0x0, 2); // zc det time
}

void DRV2605L::getRegisterData(uint8_t reg){
    if(!enabled){
       enable();
    }

    // Flush buffer
    while(Wire.available()){
        Wire.read();
    }

    // Get contents of all registers - send starting address bit of 0, then send STOP bit
    Wire.beginTransmission(DRV2605L_ADDRESS);
    Wire.write(reg);
    Wire.endTransmission(true);

    // open second transmission, and send STOP bit at the end
    Wire.requestFrom(DRV2605L_ADDRESS, 1, true);
    while(Wire.available()){
        registers[reg] = Wire.read();
    }
}

void DRV2605L::getRegisterData(){
    if(!enabled){
       enable();
    }
    
    // Flush buffer
    while(Wire.available()){
        Wire.read();
    }

    // Get contents of all registers - send starting address bit of 0, then send STOP bit
    Wire.beginTransmission(DRV2605L_ADDRESS);
    Wire.write(0);
    Wire.endTransmission(true);

    // open second transmission, and send STOP bit at the end
    Wire.requestFrom(DRV2605L_ADDRESS, NUM_DRV_REGISTERS, true);
    int x = 0;
    while(Wire.available()){
        registers[x++] = Wire.read();
    }
}

void DRV2605L::sendRegisterData(uint8_t startAddr, uint8_t len){
    if(!enabled){
       enable();
    }
    
    Wire.beginTransmission(DRV2605L_ADDRESS);
    Wire.write(startAddr);
    for(int i = 0; i < len; i++){
        Wire.write(registers[startAddr + i]);
    }
    Wire.endTransmission(true);
}

void DRV2605L::writeRegister(uint8_t address, uint8_t offset, uint8_t value, uint8_t len){
    if(!enabled){
       enable();
    }
    getRegisterData(address);
    uint8_t bitmask = ((1 << len) - 1) << offset;
    uint8_t valueToWrite = value << offset;
    uint8_t regValue = registers[address];
    regValue = (regValue & ~bitmask) | valueToWrite; 
    registers[address] = regValue;
    sendRegisterData(address, 1);
}