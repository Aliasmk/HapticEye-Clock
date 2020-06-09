#ifndef HAPTICS_HPP
#define HAPTICS_HPP

#include "pins.hpp"

#include <DRV2605L.hpp>

class Haptics {
public:
    Haptics() : haptic(MOTOR_ERM, OPEN_LOOP, PIN_DRV_EN) {}
private:
    DRV2605L haptic;

};

#endif