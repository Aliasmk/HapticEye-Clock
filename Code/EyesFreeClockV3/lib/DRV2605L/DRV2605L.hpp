#ifndef DRV2605L_H
#define DRV2605L_H

#include <stdint.h>

typedef enum MotorType{
    MOTOR_ERM,
    MOTOR_LRA
} MotorType;

typedef enum OperationMode{
    OPEN_LOOP,
    CLOSED_LOOP
} OperationMode;


/* REGISTERS */
typedef enum DRV2605L_Register {
    STATUS,
    MODE,
    RT_INPUT,
    LIB_SEL,
    WAV_1,
    WAV_2,
    WAV_3,
    WAV_4,
    WAV_5,
    WAV_6,
    WAV_7,
    WAV_8,
    GO,
    OD_TOFFSET,
    SUS_TOFFSET,
    SUS_TOFFSET_N,
    BRAKE_TOFFSET,
    A2V_CTRL,
    A2V_MIN_IN,
    A2V_MAX_IN,
    A2V_MIN_ODR,
    A2V_MAX_ODR,
    RATED_V,
    OD_CLAMP_V,
    AUTOCALIB_COMP,
    AUTOCALIB_BACKEMF,
    FEEDBACK_CTRL,
    CONTROL1,
    CONTROL2,
    CONTROL3,
    CONTROL4,
    CONTROL5,
    LRA_OL_PER,
    VBATT_MON,
    LRA_RES_PER,
    NUM_DRV_REGISTERS
} DRV2605L_Register;

typedef enum WaveformLibrary{
    NONE,
    LIBRARY_A,
    LIBRARY_B,
    LIBRARY_C,
    LIBRARY_D,
    LIBRARY_E,
    LRA_LIBRARY,
    LIBRARY_F
} WaveformLibrary;

typedef enum TriggerType{
    TRIG_INTERNAL,
    TRIG_EXTERNAL_EDGE,
    TRIG_EXTERNAL_LEVEL,
    TRIG_PWM_ANALOG,
    TRIG_A2V,
    TRIG_RTP,
    DIAGNOSTICS,
    AUTOCALIBRATION
} TriggerType;

/* OFFSETS */
// Register 0x00 (STATUS)


// Register 0x01 (MODE)
#define MODE_OFFSET 0
#define STANDBY_OFFSET 6
#define DEV_RESET 7

// Register 0x1A (FEEDBACK_CTRL)
#define N_ERM_LRA_OFFSET 7
#define FB_BRAKE_FACTOR_OFFSET 4
#define LOOP_GAIN_OFFSET 2
#define BEMF_GAIN_OFFSET 0

// Register 0x16 (RATED_VOLTAGE)
#define RATED_VOLTAGE_OFFSET 0

// Register 0x17 (OD_CLAMP)
#define OD_CLAMP_OFFSET 0

// Register 0x1E (CONTROL4)
#define AUTO_CAL_TIME_OFFSET 4

// Register 0x1B ()

#define DRV2605L_ADDRESS 0x5A 

class DRV2605L{
public:
    DRV2605L(MotorType type, OperationMode mode, int enablePin);
    void initialize();
    void enable();
    void disable();
    void setTriggerType(TriggerType trig);
    void setWaveformLibrary(WaveformLibrary lib);
    void queueWaveform(int index, int data, bool wait);
    void playWaveforms();
    void stopWaveforms();

//private:
    MotorType type_;
    OperationMode mode_;
    int enablePin_;

    bool enabled;

    uint8_t registers[NUM_DRV_REGISTERS];
    void autoCalibrate();

    void getRegisterData();
    void getRegisterData(uint8_t reg);
    void sendRegisterData(uint8_t startAddr, uint8_t len);

    void writeRegister(uint8_t address, uint8_t offset, uint8_t value, uint8_t len);
    

};


#endif