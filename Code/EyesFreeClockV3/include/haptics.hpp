#ifndef HAPTICS_HPP
#define HAPTICS_HPP

#include "pins.hpp"

#include <DRV2605L.hpp>

//Beep timings
#define BETWEENDELAY 800    //time to wait between tens and ones etc.
#define NONZEROBEEPTIME 50  //the time to beep for a non-zero number
#define ZEROBEEPTIME 300    //the time to beep for a zero
#define BEEPDELAY 200       //the spacing between beeps of the same group
#define GROUPDELAYADD 40    //the amount of delay between groupings (ie ... ... .. to make 8)

class Haptics {
public:
    Haptics();

    void init();

    void tick();

    void vibrateFree(uint16_t ms);
    void vibrateTime(uint8_t hour, uint8_t minute, bool pm);
    void stopVibration();

    void setVibeTimescale(float scale);

private:
    DRV2605L drv;

    void buzzCountOld(int count);

    float timescale;
    bool freeVibeEnabled;
    long freeVibeEndTime;
};

#endif
