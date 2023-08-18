#ifndef _MOTOR_h
#define _MOTOR_h

#include "Arduino.h"

class Motor_ {
    public:
        void begin(void);
        void loop(void);
        void setSpeed(uint8_t speed);
    private:
        uint8_t requestedSpeed;
        uint8_t currentSpeed;
        uint32_t lastMs;
};

extern Motor_ Motor;

#endif
