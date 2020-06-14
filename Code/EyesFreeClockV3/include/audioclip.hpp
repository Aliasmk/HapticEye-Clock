#ifndef AUDIOCLIP_HPP
#define AUDIOCLIP_HPP

#include <stdio.h>

// This file contains metadata about the audio clips stored on the memory

typedef enum AudioClipEnum {
    CLIP_HELLO,
    CLIP_OCLOCK,
    CLIP_AM,
    CLIP_PM,
    CLIP_ONE,
    CLIP_TWO,
    CLIP_THREE,
    CLIP_FOUR,
    CLIP_FIVE,
    CLIP_SIX,
    CLIP_SEVEN,
    CLIP_EIGHT,
    CLIP_NINE,
    CLIP_TEN,
    CLIP_ELEVEN,
    CLIP_TWELVE,
    CLIP_THIRTEEN,
    CLIP_FOURTEEN,
    CLIP_FIFTEEN,
    CLIP_SIXTEEN,
    CLIP_SEVENTEEN,
    CLIP_EIGHTEEN,
    CLIP_NINETEEN,
    CLIP_TWENTY,
    CLIP_THIRTY,
    CLIP_FOURTY,
    CLIP_FIFTY,
    NUM_CLIPS
} AudioClipEnum;

typedef struct clipMetadata{
        AudioClipEnum index;
        uint16_t startAddr;
        uint16_t size;
} clipMetadata;

const clipMetadata data[] = {
    { CLIP_HELLO, 0, 10240 }
    // TODO populate when data package complete
};

class AudioClip{
public:
    static uint16_t getClipAddress(AudioClipEnum clip){
        return data[clip].startAddr;
    } 
    static uint16_t getClipSize(AudioClipEnum clip){
        return data[clip].size;
    } 
private:
    
};

#endif