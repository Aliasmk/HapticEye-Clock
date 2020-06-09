#include <Arduino.h>

#define ATMEGA328PB


#ifdef ATMEGA328PB

#define PIN_LED         PIN_PD5
#define PIN_DAC_EN      PIN_PB1
#define PIN_EEPROM_CS   PIN_PB2
#define PIN_BTN_MODE    PIN_PC0
#define PIN_BTN_SET     PIN_PC1
#define PIN_DRV_EN      PIN_PC2

#endif