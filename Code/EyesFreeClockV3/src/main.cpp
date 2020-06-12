#include <Arduino.h>

#include "pins.hpp"

#include "HEyeO.hpp"
#include "audio.hpp"
#include "haptics.hpp"
#include "time.hpp"

#include <RTCTime.hpp>

#define BTN_HOLD_DELAY 1250

typedef enum States{
  STATE_NORMAL,
  STATE_TIME_SET,
  STATE_OUTPUT
} States;

typedef enum TimeSetStage{
  TS_HOUR,
  TS_MIN_TEN,
  TS_MIN_ONE,
  TS_AMPM
} TimeSetStage;

// TODO: Add slow haptic mode
typedef enum OutputMode{
  OUTPUT_HAPTIC,
  OUTPUT_AUDIO
} OutputMode;

HEyeO io;
Time time;
Audio audio;
Haptics haptics;

States state;
OutputMode mode;
TimeSetStage ts_stage;

void changeState(States newState){
  Serial.print("[MAIN]\tNew State: ");
  Serial.println((int)newState);
  state = newState;
  if(state == STATE_TIME_SET){
    ts_stage = TS_HOUR;
    Serial.println("[MAIN]\tSetting Hour...");
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("\r\n\nHello, from HapticEye by Michael Kafarowski.");
  
  time.init();
  haptics.init();

  if(time.timeSetRequired()){
    Serial.println("Time Set Required");
    changeState(STATE_TIME_SET);
  } else {
    time.printTime();
    Serial.println();
    changeState(STATE_NORMAL);
  }
}

void changeMode(){
  // Swap the output mode

  if(mode == OUTPUT_AUDIO){
    mode = OUTPUT_HAPTIC;
    haptics.vibrateFree(500);
    
  } else if(mode == OUTPUT_HAPTIC){
    mode = OUTPUT_AUDIO;
    // TODO audio.speak(HELLO);
  }
  Serial.print("[MAIN]\tNew Mode: ");
  Serial.println(mode);
}

void timeSet(){
  // For all:
    // If mode button pressed, increase count
    // If set button pressed, move to next digit
  
  static int hour = 12;
  static int minTens = 0;
  static int minOnes = 0;
  static bool pm = false;

  switch(ts_stage){
    case TS_HOUR:
      if(io.buttonClicked(BTN_MODE)){
        hour++;
        if(hour > 12){
          hour = 1;
        }
      } else if(io.buttonClicked(BTN_SET)){
        Serial.println("[MAIN]\tSetting Minute Tens...");
        ts_stage = TS_MIN_TEN;
      }    
    break;

    case TS_MIN_TEN:
      if(io.buttonClicked(BTN_MODE)){
        minTens++;
        if(minTens > 5){
          minTens = 0;
        }
      } else if(io.buttonClicked(BTN_SET)){
        Serial.println("[MAIN]\tSetting Minute Ones...");
        ts_stage = TS_MIN_ONE;
      }  
    break;

    case TS_MIN_ONE:
      if(io.buttonClicked(BTN_MODE)){
        minOnes++;
        if(minOnes > 9){
          minOnes = 0;
        }
      } else if(io.buttonClicked(BTN_SET)){
        Serial.println("[MAIN]\tSetting AM/PM...");
        ts_stage = TS_AMPM;
      }  
    break;

    case TS_AMPM:
      if(io.buttonClicked(BTN_MODE)){
        pm = !pm;
      } else if(io.buttonClicked(BTN_SET)){
        time.setTime(hour, minTens, minOnes, pm);
        changeState(STATE_OUTPUT);
      }  
    break;
  }
}

void outputTime(){
  Serial.println("[MAIN]\tOutputting Time");
  RTCTime now;
  time.getTime(&now);

  delay(400);

  switch(mode){
    case OUTPUT_HAPTIC:
      haptics.vibrateTime(now.hour, now.minute, now.ampm == TIME_PM ? true : false);
    break;

    case OUTPUT_AUDIO:

    break;
  }
  changeState(STATE_NORMAL);
}


unsigned long lastPingTime = 0;


void loop() {
  unsigned long now = millis();
  
  // Wait for inputs
    // If mode button held, swap audio/haptic
    // If set button held, enter time set loop
    // If mode pressed, output the time
  io.tick();
  haptics.tick();

  switch(state){
    case STATE_NORMAL:
      if(io.buttonHeld(BTN_MODE, BTN_HOLD_DELAY)){
          changeMode();
      } else if(io.buttonHeld(BTN_SET, BTN_HOLD_DELAY)){
          changeState(STATE_TIME_SET);
      } else if(io.buttonClicked(BTN_MODE)){
          changeState(STATE_OUTPUT);
      }

      if((now - lastPingTime) > 5000){
        time.printTime();
        lastPingTime = now;
      }

    break;

    case STATE_TIME_SET:
      timeSet();
    break;

    case STATE_OUTPUT:
      outputTime();
    break;
  }
}