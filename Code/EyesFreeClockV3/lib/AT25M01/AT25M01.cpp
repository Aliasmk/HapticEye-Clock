#include "AT25M01.hpp"

#include <SPI.h>

#define EEPROM_ADDR_MASK 0x1FFFF

#define EEPROM_SPI_SPEED_4V5 (uint32_t)20000000
#define EEPROM_SPI_SPEED_2V5 (uint32_t)10000000
#define EEPROM_SPI_SPEED_1V7 (uint32_t) 5000000

#define EEPROM_OPCODE_WREN  (uint8_t)0x6
#define EEPROM_OPCODE_WRDI  (uint8_t)0x4
#define EEPROM_OPCODE_RDSR  (uint8_t)0x5
#define EEPROM_OPCODE_WRSR  (uint8_t)0x1
#define EEPROM_OPCODE_READ  (uint8_t)0x3
#define EEPROM_OPCODE_WRITE (uint8_t)0x2

#define EEPROM_STATUS_READY (uint8_t)0x1
#define EEPROM_STATUS_WEN   (uint8_t)0x2

SPISettings EEPROM_SPI_Settings(EEPROM_SPI_SPEED_4V5, MSBFIRST, SPI_MODE0);

AT25M01::AT25M01(uint8_t cs) : cs_(cs){
    pinMode(cs_, OUTPUT);
    digitalWrite(cs_, HIGH);
    SPI.begin();
}

void AT25M01::write(const uint8_t *src, const uint32_t dst, uint8_t len){
    // Disable the write protection
    SPI.beginTransaction(EEPROM_SPI_Settings);
    digitalWrite(cs_, LOW);
    SPI.transfer(EEPROM_OPCODE_WREN);
    digitalWrite(cs_, HIGH);
    SPI.endTransaction();

    // Begin the write transmission
    SPI.beginTransaction(EEPROM_SPI_Settings);
    digitalWrite(cs_, LOW);
    
    // Transfer the write opcode
    SPI.transfer(EEPROM_OPCODE_WRITE);

    // Transfer the address to write to
    SPI.transfer((uint8_t)((dst&0x10000)>>16));
    SPI.transfer((uint8_t)((dst&0x0FF00)>>8));
    SPI.transfer((uint8_t) (dst&0x000FF));

    // Transfer the data
    for(int i = 0; i<len; i++){
        SPI.transfer((uint8_t)src[i]);     
    }

    // End the transmission
    digitalWrite(cs_, HIGH);
    SPI.endTransaction();
}

uint8_t AT25M01::read(const uint8_t src){
    uint8_t value;
    
    // Begin the write transmission
    SPI.beginTransaction(EEPROM_SPI_Settings);
    digitalWrite(cs_, LOW);
    
    // Transfer the write opcode
    SPI.transfer(EEPROM_OPCODE_READ);

    // Transfer the address to read from
    SPI.transfer((uint8_t)((src&0x10000)>>16));
    SPI.transfer((uint8_t)((src&0x0FF00)>>8));
    SPI.transfer((uint8_t) (src&0x000FF));
    
    // Continuously transfer data to buffer
    value = SPI.transfer(0x0);

    // End the transmission
    digitalWrite(cs_, HIGH);
    SPI.endTransaction();

    return value;
}

void AT25M01::read(const uint8_t src, uint8_t *dst, uint32_t len){
    // Begin the write transmission
    SPI.beginTransaction(EEPROM_SPI_Settings);
    digitalWrite(cs_, LOW);
    
    // Transfer the write opcode
    SPI.transfer(EEPROM_OPCODE_READ);

    // Transfer the address to read from
    SPI.transfer((uint8_t)((src&0x10000)>>16));
    SPI.transfer((uint8_t)((src&0x0FF00)>>8));
    SPI.transfer((uint8_t) (src&0x000FF));
    
    // Continuously transfer data to buffer
    SPI.transfer(dst, len);

    // End the transmission
    digitalWrite(cs_, HIGH);
    SPI.endTransaction();
}

bool AT25M01::isReady(){
    if((readStatus() & EEPROM_STATUS_READY) == 0){ //bit 0 = 0 indicates the device is ready
        return true;
    } else {
        return false;
    }
}

uint8_t AT25M01::readStatus(){
    uint8_t sr = 0xA5;
    
    SPI.beginTransaction(EEPROM_SPI_Settings);
    digitalWrite(cs_, LOW);
    
    // Transfer the write opcode
    SPI.transfer(EEPROM_OPCODE_RDSR);
    sr = SPI.transfer(0);

    digitalWrite(cs_, HIGH);
    SPI.endTransaction();

    return sr;
}