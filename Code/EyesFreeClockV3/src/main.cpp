#include <Arduino.h>

#include <DRV2605L.hpp>
#include <MCP7940N.hpp>
#include <AT25M01.hpp>
#include <LTC2630.hpp>

#define PIN_LED 5
#define PIN_DAC_EN 9
#define PIN_EEPROM_CS 10
#define PIN_BTN_MODE 14
#define PIN_BTN_SET 15
#define PIN_DRV_EN 16


DRV2605L haptic(MOTOR_ERM, OPEN_LOOP, PIN_DRV_EN);
MCP7940N rtc(EXT_XTAL, true);
AT25M01 eeprom(PIN_EEPROM_CS);
LTC2630 dac(PIN_DAC_EN, DAC_MODE_10BIT);

void initDance(){
  haptic.setWaveformLibrary(LIBRARY_F);
  haptic.queueWaveform(0, 1, false);
  haptic.queueWaveform(1, 15, true);
  haptic.queueWaveform(2, 1, false);
  haptic.queueWaveform(3, 20, true);
  haptic.queueWaveform(4, 12, false);
  haptic.playWaveforms();
  for(int i = 0; i<8; i++){
    analogWrite(PIN_LED, 1<<i);
    delay(45);
  }
  for(int i = 6; i>=0; i--){
    analogWrite(PIN_LED, 1<<i);
    delay(45);
  }
  digitalWrite(PIN_LED, LOW);
  
  uint8_t sinedata[255];
  for(int i = 0; i<255; i++){
    sinedata[i] = (128 * sin(i * 1.01025)) + 128;
  }
  
  Serial.print("DATA Test: ");
  for(int i = 0; i < 255; i++){
    Serial.print(sinedata[i]);
    Serial.print(", ");
  }
  
  eeprom.write(sinedata, 0, 255);
  
  uint8_t readData[255];
  eeprom.read(0, readData, 255);

  Serial.print("EEPROM Test: ");
  for(int i = 0; i < 255; i++){
    Serial.print(eeprom.read(i));
    Serial.print(", ");
  }

  Serial.println();
  
  for(int j = 0; j < 3; j++){
    for(int i = 0; i < 255; i++){
      uint16_t data = (eeprom.read(i)<<1) + 384;
      dac.write(data);
      delayMicroseconds(20);
    }
  }
  delay(50);
  for(int j = 0; j < 4; j++){
    for(int i = 0; i < 255; i++){
      uint16_t data = (eeprom.read(i)<<1) + 384;
      dac.write(data);
      delayMicroseconds(20);
    }
  }

  dac.write(512);

  delay(500);
}

void setup() {
  Serial.begin(115200);
  Serial.println("\r\n\nHello, from HapticITD by Michael Kafarowski.\n");
  
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BTN_MODE, INPUT);
  pinMode(PIN_BTN_SET, INPUT);
  
  haptic.initialize();
  rtc.initialize(); 
  initDance();

  Serial.println("Hardware Init Complete.");

  if(rtc.readSRAM(0x20) != 0x01){
    // Time has not yet been set, so set it now.
    Serial.println("Time not yet set - setting now.");
    RTCTime newTime;
    newTime.ampm = TIME_24;
    newTime.hour = 23;
    newTime.minute = 59;
    newTime.second = 23; 
    rtc.setTimeAndDate(&newTime);
    Serial.println("Time was set.");
    rtc.writeSRAM(0x20, 0x01);
  } else {
    Serial.println("Time was previously set.");
  }
}

long lastTimeLED = 0;
long lastTimeRTC = 0;

void loop() {
  long now = millis();

  if(now - lastTimeLED > 1000){
    lastTimeLED = now;
    digitalWrite(PIN_LED, !digitalRead(PIN_LED));
  }

  if(now - lastTimeRTC > 5000){
    lastTimeRTC = now;
    RTCTime currentTime;
    rtc.getTimeAndDate(&currentTime);
    Serial.print("Current Time: ");
    Serial.print(currentTime.hour);
    Serial.print(":");
    Serial.print(currentTime.minute);
    Serial.print(":");
    Serial.println(currentTime.second);
  }
  
}