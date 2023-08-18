#ifndef _CONTROL_h
#define _CONTROL_h

#include "Arduino.h"

#include "Input.hpp"

class Control_ {
    public:
        void begin(void);
    private:
        static bool motorOn;
        static int speed;
        static void buttonHandlerEncoder(InputEvent event);
        static void encoderHandler(int diff);
};

extern Control_ Control;

#endif
