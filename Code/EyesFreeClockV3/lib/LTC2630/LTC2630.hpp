#ifndef LTC2630_H
#define LTC2630_H

#include <stdint.h>


typedef enum DAC_DataMode {
    DAC_MODE_8BIT,
    DAC_MODE_10BIT,
    DAC_MODE_12BIT
} DAC_DataMode;

typedef enum DAC_RefMode {
    DAC_REF_INTERNAL,
    DAC_REF_EXTERNAL
} DAC_RefMode;



class LTC2630{
public:
    LTC2630(uint8_t cs, DAC_DataMode mode);

    void write(const uint16_t data);
    void write(const uint16_t data, bool update);
    void update();

    void setRefMode(DAC_RefMode mode);
    

private:
    uint8_t cs_;
    DAC_DataMode mode_;
};

#endif