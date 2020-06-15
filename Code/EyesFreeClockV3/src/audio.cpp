#include "audio.hpp"

// TODO make asynchronous with timers
void Audio::speak(AudioClipEnum clip){
    uint32_t frame = 0;
    while(frame < AudioClip::getClipSize(clip)){
        PORTD |= B00100000;
        dac.write(mem.read(AudioClip::getClipAddress(clip) + frame) << 2);
        PORTD &= ~B00100000;
        frame++;
        delayMicroseconds(55);
    }
}

void Audio::speakNumber(int number){
  speakNumber(number, true);
}


void Audio::speakNumber(int number, bool oh){
  if(number < 0 || number >= 60){
    return;
  }

  if (number > 0 && number < 10)
  {
    if(oh){
      speak(CLIP_OH); 
    }
    
  }
  else if (number >= 10 && number < 20)
  {
    //10 to 19
    // Say nothing (teens)
  }
  else if (number >= 20 && number < 30)
  {
    //20 to 29
    speak(CLIP_TWENTY);
    number = number % 10;
  }
  else if (number >= 30 && number < 40)
  {
    //30 to 39
    speak(CLIP_THIRTY);
    number = number % 10;
  }
  else if (number >= 40 && number < 50)
  {
    //40 to 49
    speak(CLIP_FOURTY);
    number = number % 10;
  }
  else if (number >= 50 && number < 60)
  {
    //50 to 59
    speak(CLIP_FIFTY);
    number = number % 10;
  }

  if (number == 0)
  {
    // Say nothing skip to ampm
  }
  else if (number == 1)
  {
    speak(CLIP_ONE);
  }
  else if (number == 2)
  {
    speak(CLIP_TWO);
  }
  else if (number == 3)
  {
    speak(CLIP_THREE);
  }
  else if (number == 4)
  {
    speak(CLIP_FOUR);
  }
  else if (number == 5)
  {
    speak(CLIP_FIVE);
  }
  else if (number == 6)
  {
    speak(CLIP_SIX);
  }
  else if (number == 7)
  {
    speak(CLIP_SEVEN);
  }
  else if (number == 8)
  {
    speak(CLIP_EIGHT);
  }
  else if (number == 9)
  {
    speak(CLIP_NINE);
  }
  else if (number == 10)
  {
    speak(CLIP_TEN);
  }
  else if (number == 11)
  {
    speak(CLIP_ELEVEN);
  }
  else if (number == 12)
  {
    speak(CLIP_TWELVE);
  }
  else if (number == 13)
  {
    speak(CLIP_THIRTEEN);
  }
  else if (number == 14)
  {
    speak(CLIP_FOURTEEN);
  }
  else if (number == 15)
  {
    speak(CLIP_FIFTEEN);
  }
  else if (number == 16)
  {
    speak(CLIP_SIXTEEN);
  }
  else if (number == 17)
  {
    speak(CLIP_SEVENTEEN);
  }
  else if (number == 18)
  {
    speak(CLIP_EIGHTEEN);
  }
  else if (number == 19)
  {
    speak(CLIP_THIRTEEN);
  }
}

void Audio::speakTime(uint8_t hour, uint8_t minute, bool pm){
// Speak hours
  speakNumber(hour, false);
  if(minute > 0){
    speakNumber(minute);
  }

  // Speak AM/PM
  if(pm){
    speak(CLIP_PM);
  } else {
    speak(CLIP_AM);
  }
}