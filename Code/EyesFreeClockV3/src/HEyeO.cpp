#include <Arduino.h>

#include "HEyeO.hpp"

// Get it? The IO class is called HapticEyeOh? 
// io.hpp was already in use...

// NOTE: Assumes buttons ACTIVE LOW

HEyeO::HEyeO(){
    pinMode(PIN_BTN_MODE, INPUT);
    pinMode(PIN_BTN_SET, INPUT);
    pinMode(PIN_DAC_EN, OUTPUT);
    pinMode(PIN_EEPROM_CS, OUTPUT);

    for(int i = 0; i < NUM_BUTTONS; i++){
        btnStatus[i].btn = (Button)i;
        btnStatus[i].clicked = false;
        btnStatus[i].lastState = HIGH; //Buttons start out high
        btnStatus[i].timeSwitched = 0;
        btnStatus[i].holdLock = false;
    }

}

void HEyeO::tick(){
    long now = millis();
    
    // Poll the states of each button in the button list
    for(int i = 0; i < NUM_BUTTONS; i++){
        
    
        int buttonPin = getPin((Button)i);
        bool currentButtonState = digitalRead(buttonPin);
        
        // Check for state changes
        if(currentButtonState != btnStatus[i].lastState){
            // Button state has changed
            
            if((now - btnStatus[i].timeSwitched) > DEBOUNCE_TIME_MIN_MS){
                Serial.print(F("[IO]\tChange in Button "));
                Serial.print(i);
                Serial.print(F(" - "));
                // State change probably wasn't a bounce
                
                if(currentButtonState == HIGH){
                    // Button has been released
                    btnStatus[i].holdLock = false;
                    
                    Serial.print(F("released "));
                    if((now - btnStatus[i].timeSwitched) < CLICK_TIME_MAX_MS){
                        // Button was clicked
                        btnStatus[i].clicked = true;
                        Serial.print(F("(click)"));
                    } else {
                        Serial.print(F("(held "));
                        Serial.print(now - btnStatus[i].timeSwitched);
                        Serial.print(F(" ms)"));
                    }
                } else {
                    // Button has been pressed
                    Serial.print(F("pressed"));
                    btnStatus[i].clicked = false;
                }

                Serial.println();

                // Store this state change information
                btnStatus[i].lastState = currentButtonState;
                btnStatus[i].timeSwitched = now;
            }
        }

       
    }


}

bool HEyeO::buttonHeld(Button btn, int delay){
    long now = millis();
    if((now - btnStatus[(int)btn].timeSwitched) > delay && btnStatus[(int)btn].lastState == LOW && btnStatus[(int)btn].holdLock == false){
        Serial.print(F("[IO]\tButton held longer than requested delay ("));
        Serial.print(delay);
        Serial.println(F(" ms)"));
        btnStatus[(int)btn].holdLock = true;
        return true;
    } else {
        return false;
    }
}

bool HEyeO::buttonClicked(Button btn){
    if(btnStatus[(int)btn].clicked == true && btnStatus[(int)btn].holdLock == false){
        btnStatus[(int)btn].clicked = false;
        return true;
    } else {
        return false;
    }
}

bool HEyeO::buttonDown(Button btn){
    if(btnStatus[(int)btn].lastState == LOW){
        return true;
    } else {
        return false;
    }
}

bool HEyeO::buttonUp(Button btn){
    if(btnStatus[(int)btn].lastState == HIGH){
        return true;
    } else {
        return false;
    }
}

uint8_t HEyeO::getPin(Button btn){
    for(int i = 0; i < NUM_BUTTONS; i++){
        if(btnMap[i].btn == btn){
            return btnMap[i].pin;
        }
    }
    return 0;
}

ButtonStatus* HEyeO::getButtonStatus(Button btn){
    for(int i = 0; i < NUM_BUTTONS; i++){
        if(btnStatus[i].btn == btn){
            return &btnStatus[i];
        }
    }
    return nullptr;
}