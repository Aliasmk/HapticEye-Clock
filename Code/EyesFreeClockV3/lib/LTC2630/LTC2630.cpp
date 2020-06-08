#include "LTC2630.hpp"

#include <SPI.h>

#define DAC_CMD_WRITE   (uint8_t)0x0
#define DAC_CMD_UPDATE  (uint8_t)0x1
#define DAC_CMD_WRUP    (uint8_t)0x3
#define DAC_CMD_PWRDWN  (uint8_t)0x4
#define DAC_CMD_SELINT  (uint8_t)0x6
#define DAC_CMD_SELEXT  (uint8_t)0x7

SPISettings DAC_SPI_Settings(20000000, MSBFIRST, SPI_MODE0);

LTC2630::LTC2630(uint8_t cs, DAC_DataMode mode) : 
    cs_(cs),
    mode_(mode)
{
    pinMode(cs_, OUTPUT);
    digitalWrite(cs_, HIGH);
    SPI.begin();
}


void LTC2630::write(const uint16_t data, bool update){
    SPI.beginTransaction(DAC_SPI_Settings);
    digitalWrite(cs_, LOW);

    int dataShift;
    if(mode_ == DAC_MODE_8BIT)
        // 8 bit data mode
        dataShift = 8;
    else if(mode_ == DAC_MODE_10BIT){
        // 10 bit data mode
        dataShift = 6;
    } else {
        // 12 bit data mode
        dataShift = 4;
    }

    // Send the write command and the data to be converted
    if(update){
        // Write and Update the DAC
        SPI.transfer(DAC_CMD_WRUP << 4);
    } else {
        // Write to the registers, but don't update the DAC
        SPI.transfer(DAC_CMD_WRITE << 4);
    }
    SPI.transfer16(data << dataShift);

    digitalWrite(cs_, HIGH);
    SPI.endTransaction();
}

void LTC2630::write(const uint16_t data){
    write(data, true);
}

void LTC2630::update(){
    SPI.beginTransaction(DAC_SPI_Settings);
    digitalWrite(cs_, LOW);

    SPI.transfer(DAC_CMD_UPDATE);
    SPI.transfer16(0); // Transfer 16 bits to meet the minimum 24 bit requirement

    digitalWrite(cs_, HIGH);
    SPI.endTransaction();
}

void LTC2630::setRefMode(DAC_RefMode mode){
    SPI.beginTransaction(DAC_SPI_Settings);
    digitalWrite(cs_, LOW);

    if(mode == DAC_REF_INTERNAL){
        SPI.transfer(DAC_CMD_SELINT);
    } else {
        SPI.transfer(DAC_CMD_SELEXT);
    }
    SPI.transfer16(0); // Transfer 16 bits to meet the minimum 24 bit requirement
    
    digitalWrite(cs_, HIGH);
    SPI.endTransaction();
}



