#include "audio.hpp"

void Audio::speak(AudioClipEnum clip){
    uint16_t frame = 0;
    while(frame < AudioClip::getClipSize(clip)){
        PORTD |= B00100000;
        dac.write(mem.read(AudioClip::getClipAddress(clip) + frame) << 2);
        PORTD &= ~B00100000;
        frame++;
        delayMicroseconds(55);
    }
}