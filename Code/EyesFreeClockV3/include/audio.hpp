#ifndef AUDIO_HPP
#define AUDIO_HPP

#include "pins.hpp"
#include "audioclip.hpp"

#include <AT25M01.hpp>
#include <LTC2630.hpp>

class Audio {
public:
    Audio() : mem(PIN_EEPROM_CS), dac(PIN_DAC_EN, DAC_MODE_10BIT) {}

    void speak(AudioClipEnum clip);
    void speakTime(uint8_t hour, uint8_t minute, bool pm);
    void speakNumber(int number);
    void speakNumber(int number, bool oh);
private:
    AT25M01 mem;
    LTC2630 dac;

    bool audioPlaying;
    
    uint16_t audioFIFO[64];
    uint16_t* fifoIndex;
};

#endif