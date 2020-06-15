#ifndef AUDIOCLIP_HPP
#define AUDIOCLIP_HPP

#include <stdio.h>

// This file contains metadata about the audio clips stored on the memory

typedef enum AudioClipEnum {
    CLIP_HELLO,
    CLIP_AM,
    CLIP_PM,
    CLIP_OH,
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
    CLIP_HOUR,
    CLIP_MINUTES,
    CLIP_SET,
    NUM_CLIPS
} AudioClipEnum;

typedef struct clipMetadata{
        AudioClipEnum index;
        uint32_t startAddr;
        uint32_t size;
} clipMetadata;

const clipMetadata data[] = {
    { CLIP_HELLO, 0, 4096},
    { CLIP_AM, 4096, 4352},
    { CLIP_PM, 8448, 4096},
    { CLIP_OH, 12544, 3072},
    { CLIP_ONE, 15616, 4352},
    { CLIP_TWO, 19968, 3840},
    { CLIP_THREE, 23808, 3584},
    { CLIP_FOUR, 27392, 3328},
    { CLIP_FIVE, 30720, 3584},
    { CLIP_SIX, 34304, 2560},
    { CLIP_SEVEN, 36864, 3328},
    { CLIP_EIGHT, 40192, 2048},
    { CLIP_NINE, 42240, 4352},
    { CLIP_TEN, 46592, 4096},
    { CLIP_ELEVEN, 50688, 4864},
    { CLIP_TWELVE, 55552, 4352},
    { CLIP_THIRTEEN, 59904, 5632},
    { CLIP_FOURTEEN, 65536, 6144},
    { CLIP_FIFTEEN, 71680, 6144},
    { CLIP_SIXTEEN, 77824, 6400},
    { CLIP_SEVENTEEN, 84224, 6912},
    { CLIP_EIGHTEEN, 91136, 6656},
    { CLIP_NINETEEN, 97792, 7424},
    { CLIP_TWENTY, 105216, 5120},
    { CLIP_THIRTY, 110336, 3584},
    { CLIP_FOURTY, 113920, 3584},
    { CLIP_FIFTY, 117504, 4352},
    { CLIP_HOUR, 121856, 3072},
    { CLIP_MINUTES, 124928, 2560},
    { CLIP_SET, 127488, 2305}
};

class AudioClip{
public:
    static uint32_t getClipAddress(AudioClipEnum clip){
        return data[clip].startAddr;
    } 
    static uint32_t getClipSize(AudioClipEnum clip){
        return data[clip].size;
    } 
private:
    
};

#endif