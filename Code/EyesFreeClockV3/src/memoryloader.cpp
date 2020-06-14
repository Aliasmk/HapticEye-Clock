#include <Arduino.h>

#include <AT25M01.hpp>
#include "memoryloader.hpp"
#include "pins.hpp"

void MemoryLoader::start(){
    Serial.println(F("Memory Loader Activated"));
    AT25M01 mem(PIN_EEPROM_CS);
    bool moreFiles = true;
    int size = 0;
    int chunks = 0;
    int remainder = 0;
    int startAddr = 0;
    char buffer[32];

    
    while(moreFiles){
        // First we'll query the size and start address of the incoming "file"
        // Right now it's up to you to know the sizes and addresses of the stored files, as there is no filesystem/manifest
        Serial.println(F("Ready. Enter size:"));
        getSerialInput(buffer, 32);
        size = atoi(buffer);

        if(size % 256 != 0){
            int chunks = size / 256;
            size = 256 * chunks;
            snprintf(buffer, 32, "[NOTE] Floored to %u", size);
        } else {
            snprintf(buffer, 32, "OK, %d", size);
        }
        
        Serial.println(buffer);

        do{
            Serial.println(F("Enter Start Addr: "));
            getSerialInput(buffer, 32);
            startAddr = atoi(buffer);
            if(startAddr % 256 != 0){
                Serial.println(F("[ERROR] Start address must be a multiple of 256")); 
            }
        } while(startAddr % 256 != 0);

        snprintf(buffer, 32, "OK, %d. Send Data Now.", startAddr);
        Serial.println(buffer);
        // Serial is 9600 baud, so it sends at 1.2KB per second. 
        // SPI sends a total of 24 bits but operates at 15MHz, which comes out to 625KB per second
        int count = 0;
        int bytesTransferred = 0;
        uint8_t indata[256];
        while(bytesTransferred < size){
            if(Serial.available()){
                indata[count] = (uint8_t)Serial.read();
                count++;
                
                if(count == 128){
                    mem.write(&indata[0], startAddr+bytesTransferred, 128);
                    bytesTransferred+=128;
                } else if(count == 256){
                    sprintf(buffer, "\rW: %d%%", (int)((bytesTransferred/(float)size)*100));
                    Serial.print(buffer);
                    mem.write(&indata[128], startAddr+bytesTransferred, 128);
                    bytesTransferred+=128;
                    count = 0;
                }
            }
        }
        Serial.println();

        while(Serial.available()){
            Serial.read(); //discard any remaining incoming bytes
        }

        // Read back data
        Serial.println(F("Transfer Complete. Verify first 100:"));
        for(int i = 0; i < 100; i++){
            //Serial.print((char)mem.read(startAddr+i));
            if(i%16 == 0){
                Serial.println();
            }
            sprintf(buffer, "0x%02X%02X,   ", mem.read(startAddr + i), mem.read(startAddr + i++) );
            Serial.print(buffer);
        }
        Serial.println();
        

        Serial.println(F("Another? (y/n , then enter)"));
        getSerialInput(buffer, 32);
        if(buffer[0] == 'n'){
            moreFiles = false;
        }
    }
    Serial.println(F("Please manually reset."));
    while(1){};

}

void MemoryLoader::getSerialInput(char* dst, int bsize){
    bool cmdcplt = false;
    int count = 0;
    char buffer[32] = {0};
    while(!Serial.available()){}
    while(!cmdcplt){
        if(Serial.available()){
            buffer[count] = Serial.read();
            if(buffer[count] == '\r' || buffer[count] == '\n'){
                buffer[count] = '\0';
                cmdcplt = true;
            } else {
                count++;
            }
        }
    }
    strncpy(dst, buffer, bsize);
}