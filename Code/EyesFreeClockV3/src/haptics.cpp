#include "haptics.hpp"

#include "haptics.hpp"
#include "TS2200_Waveforms.hpp"

Haptics::Haptics()  : drv(MOTOR_ERM, OPEN_LOOP, PIN_DRV_EN) {
    timescale = 1;
    freeVibeEnabled = false;
}

void Haptics::init(){
    drv.initialize();
    drv.setWaveformLibrary(LIBRARY_F);
    drv.stopWaveforms();
}

void Haptics::tick(){
    long now = millis();
    if(freeVibeEnabled && (now > freeVibeEndTime)){
        // Free vibration time expired
        Serial.println("[VIBE] Vibe expired!");
        stopVibration();
        freeVibeEnabled = false;
    }

}

void Haptics::vibrateFree(uint16_t vibeTime){
    Serial.print("[VIBE]\t Free vibin' for ");
    Serial.print(vibeTime);
    Serial.println(" ms");
    long now = millis();
    freeVibeEnabled = true;
    freeVibeEndTime = now + vibeTime;
    drv.queueWaveform(0, VIBE_FREE, false);
    drv.playWaveforms();
}

void Haptics::stopVibration(){
    drv.stopWaveforms();
}

void Haptics::vibrateTime(uint8_t hour, uint8_t minute, bool pm){
    buzzCountOld(hour);
    delay((int)(BETWEENDELAY * timescale));
    buzzCountOld(minute / 10);
    delay((int)(BETWEENDELAY * timescale));
    buzzCountOld(minute % 10);
}

void Haptics::buzzCountOld(int count){
    uint8_t groupBreakdown = 3;

    if(count == 0){
        drv.queueWaveform(0, VIBE_STRONG_BUZZ, false);
        drv.playWaveforms();
        delay((int)(ZEROBEEPTIME * timescale));
        drv.stopWaveforms();
        delay((int)(BEEPDELAY * timescale));
    }
        
    for(int i = 1; i<=count; i++){
        drv.queueWaveform(0, VIBE_STRONG_CLICK, false);
        drv.playWaveforms();
        delay((int)(NONZEROBEEPTIME * timescale));
        drv.stopWaveforms();
        delay((int)(BEEPDELAY * timescale));
        if(i%groupBreakdown == 0){
        delay((int)(GROUPDELAYADD * timescale));
        }
    }

}