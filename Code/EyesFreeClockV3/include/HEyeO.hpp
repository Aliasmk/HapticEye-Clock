#ifndef HEYEO_HPP
#define HEYEO_HPP
// Get it? The IO class is called HapticEyeOh?
// io.hpp was already in use...

#include "pins.hpp"

#define DEBOUNCE_TIME_MIN_MS 100
#define CLICK_TIME_MAX_MS 500

typedef enum Button{
    BTN_MODE,
    BTN_SET,
    NUM_BUTTONS
} Button;

typedef struct ButtonMap{
    Button btn;
    uint8_t pin;
} ButtonMap;

typedef struct ButtonStatus{
    long timeSwitched;
    Button btn;
    bool lastState;
    bool clicked;
    bool holdLock;
} ButtonStatus;

class HEyeO{
public:
    HEyeO();
    
    void tick();

    bool buttonHeld(Button btn, int delay);
    bool buttonClicked(Button btn);
    bool buttonDown(Button btn);
    bool buttonUp(Button btn);

private:
    ButtonStatus btnStatus[NUM_BUTTONS];
    ButtonStatus* getButtonStatus(Button btn);
    
    const ButtonMap btnMap[2] = {
        {BTN_MODE, PIN_BTN_MODE},
        {BTN_SET, PIN_BTN_SET}
    };
    uint8_t getPin(Button btn);



    
};


#endif