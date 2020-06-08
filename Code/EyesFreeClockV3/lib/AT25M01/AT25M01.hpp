#ifndef AT25M01_H
#define AT25M01_H

#include <stdint.h>

typedef enum AT25M01_BlockProtect  {
    EEPROM_PROTECT_NONE,
    EEPROM_PROTECT_QUARTER,
    EEPROM_PROTECT_HALF,
    EEPROM_PROTECT_ALL
} AT25M01_BlockProtect;

class AT25M01{
public:
    AT25M01(uint8_t cs);
    void write(const uint8_t *src, const uint32_t dst, uint8_t len);
    void read(const uint8_t src, uint8_t *dst, uint32_t len);
    uint8_t read(const uint8_t src);
    void setBlockProtect(AT25M01_BlockProtect wpType);
    bool isReady();


private:
    const uint8_t cs_;
    uint8_t readStatus();
};



#endif