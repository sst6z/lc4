#ifndef _INPUT_h
#define _INPUT_h

#include "Arduino.h"

#define INPUT_NUM_PINS  7

enum InputEvent {
    INPUT_EVENT_PRESSED,
    INPUT_EVENT_HELD,
    INPUT_EVENT_RELEASED
};

enum InputButton {
    INPUT_BUTTON_ENCODER = 0,
    INPUT_BUTTON_MIDDLE,
    INPUT_BUTTON_UP,
    INPUT_BUTTON_DOWN,
    INPUT_BUTTON_LEFT,
    INPUT_BUTTON_RIGHT,
    INPUT_BUTTON_SPARE
};

class Input_ {
    public:
        Input_(void);
        void begin(void);
        void loop(void);
        void setButtonHandler(InputButton button, void (*handler)(InputEvent event));
        void setEncoderHandler(void (*handler)(int diff));
    private:
        struct ButtonState {
            int level;
            uint32_t lastMs;
            uint32_t repeatCount;
            void (*handler)(InputEvent event);
        };
        struct EncoderState {
            int levelA;
            int levelB;
            int state;
            volatile unsigned int idx;
            unsigned int oldIdx;
            void (*handler)(int diff);
        };
        ButtonState buttonStates[INPUT_NUM_PINS];
        const int pins[INPUT_NUM_PINS];

        static void encoderAInterrupt(void);
        static void encoderBInterrupt(void);
        static EncoderState encoderState;
};

extern Input_ Input;

#endif
