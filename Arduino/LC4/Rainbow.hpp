#ifndef _RAINBOW_h
#define _RAINBOW_h

#include "Arduino.h"

class Rainbow_ {
    public:
        void begin(void);
        void loop(void);
    private:
        uint16_t offset;
};

extern Rainbow_ Rainbow;

#endif
