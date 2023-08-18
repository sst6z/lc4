#ifndef _ENCODER_h
#define _ENCODER_h

#include "Arduino.h"

class Encoder_ {
    public:
        void begin(void);
        float getRpm(void);
    private:
        static void interruptEncoderA(void);
        static void interruptEncoderB(void);
        static void interruptSync(void);
        static volatile float rpm;
        static volatile uint32_t lastUsEncoder;
        static volatile uint32_t lastUsSync;
        static volatile uint32_t y;
        static volatile bool intBTriggered;
};

extern Encoder_ Encoder;

#endif
