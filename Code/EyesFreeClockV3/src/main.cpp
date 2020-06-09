#include <Arduino.h>

#include "pins.hpp"

#include "HEyeO.hpp"
#include "audio.hpp"
#include "haptics.hpp"
#include "time.hpp"

#define BTN_HOLD_DELAY 2500

typedef enum States{
  STATE_NORMAL,
  STATE_TIME_SET,
  STATE_OUTPUT
} States;

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

void setup() {
  Serial.begin(115200);
  Serial.println("\r\n\nHello, from HapticEye by Michael Kafarowski.\n");
  state = STATE_NORMAL;
}

void changeMode(){
  // Swap the output mode

  if(mode == OUTPUT_AUDIO){
    mode = OUTPUT_HAPTIC;
    // TODO haptic.vibrate(VIBE_SHORT_BLIP);
    
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
  
  // Hour:
    // Wait for mode button, speak entire hour when incremented
    // Wait for set button to move on

  // Mins (tens)
    // Wait for mode button, speak tens digit of minute when incremented
    // Wait for set button to move on

  // Mins (ones)
    // Wait for mode button, speak ones digit of minute when incremented
    // Wait for set button to move on

  // AM/PM
    // Speak AM
    // Wait for mode button, change AM/PM and speak
    // Wait for set button to move on

  // Confirmation
    // Speak entire time

  // Return to normal mode
}

void outputTime(){
  // Get the current time

  // If mode is haptic:
    // Instruct haptic processor to queue up the waveforms

  // If mode is audio:
    // Instruct audio to read the time value

}

void changeState(States newState){
  Serial.print("[MAIN]\tNew State: ");
  Serial.println((int)newState);
  state = newState;
}


void loop() {
  // Wait for inputs
    // If mode button held, swap audio/haptic
    // If set button held, enter time set loop
    // If mode pressed, output the time
  io.tick();

  switch(state){
    case STATE_NORMAL:
      if(io.buttonHeld(BTN_MODE, BTN_HOLD_DELAY)){
          changeMode();
      } else if(io.buttonHeld(BTN_SET, BTN_HOLD_DELAY)){
          changeState(STATE_TIME_SET);
      } else if(io.buttonClicked(BTN_MODE)){
          changeState(STATE_OUTPUT);
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