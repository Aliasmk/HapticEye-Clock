#include <Arduino.h>
#include <unity.h>
#include <AT25M01.hpp>
#include <DRV2605L.hpp>

#define PIN_DRV_EN 16
#define PIN_EEPROM_CS 10
#define FIRST_ADDRESS 0x0

const char* dataToWrite = "Hello, this is a test! 123456789";
const int msgSize = 33;

AT25M01 mem(PIN_EEPROM_CS);
DRV2605L haptic(MOTOR_ERM, CLOSED_LOOP, PIN_DRV_EN);

void test_addr_range(void){
    TEST_ASSERT_TRUE(FIRST_ADDRESS<0x1FFFF);
}

void test_msg_size(void){
    int x = 0;
    while(dataToWrite[x++] != '\0');
    TEST_ASSERT_EQUAL(msgSize,x);
}

void test_mem_cs_high(void){
    TEST_ASSERT_EQUAL(HIGH, digitalRead(PIN_EEPROM_CS));
}

void test_mem_read(void){
    char readBack[msgSize];
    mem.read(FIRST_ADDRESS, (uint8_t*)&readBack, msgSize);
    TEST_ASSERT_EQUAL_STRING_LEN(dataToWrite, readBack, msgSize);
}

void test_drv_read_reg(void){
    TEST_ASSERT_EQUAL(0x36, haptic.registers[0x1A]);
}

void test_drv_readback_reg(void){
    TEST_ASSERT_EQUAL(0x55, haptic.registers[0x1A]);
}

void test_drv_autocalib_result(void){
    TEST_ASSERT_EQUAL(0x08, haptic.registers[0x0] & 0x08);
}

void setup(){
    delay(2000);
    UNITY_BEGIN();
    RUN_TEST(test_addr_range);
    RUN_TEST(test_msg_size);
}

void loop(){
    RUN_TEST(test_mem_cs_high);
    mem.write((uint8_t*)dataToWrite, FIRST_ADDRESS, msgSize);
    RUN_TEST(test_mem_cs_high);
    while(!mem.isReady()){  }
    RUN_TEST(test_mem_read);

    haptic.writeRegister(0x1A,0x0,0x55,8);
    haptic.getRegisterData();
    RUN_TEST(test_drv_readback_reg);
    haptic.initialize();
    delay(2000);
    RUN_TEST(test_drv_autocalib_result);
    
    haptic.setWaveformLibrary(LIBRARY_F);

    haptic.queueWaveform(0, 0, false);
    haptic.queueWaveform(1, 29, false);
    haptic.queueWaveform(2, 28, false);
    haptic.queueWaveform(3, 27, false);
    haptic.queueWaveform(4, 26, false);
    haptic.queueWaveform(5, 25, false);
    haptic.queueWaveform(6, 24, false);
    haptic.queueWaveform(7, 27, false);

    haptic.playWaveforms();

    
    UNITY_END();
}