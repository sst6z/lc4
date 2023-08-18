#include "Input.hpp"

#define PIN_ENCODER_A       15
#define PIN_ENCODER_B       16
#define PIN_BUTTON_ENCODER  17
#define PIN_BUTTON_MIDDLE   9
#define PIN_BUTTON_UP       7
#define PIN_BUTTON_DOWN     10
#define PIN_BUTTON_LEFT     8
#define PIN_BUTTON_RIGHT    6
#define PIN_BUTTON_SPARE    14

#define DEBOUNCE_TIME       100
#define HELD_TIME           500
#define REPEAT_TIME         100

Input_::EncoderState Input_::encoderState = {HIGH, HIGH, 0, 0, 0, NULL};

Input_::Input_(void) : pins{PIN_BUTTON_ENCODER,
                            PIN_BUTTON_MIDDLE,
                            PIN_BUTTON_UP,
                            PIN_BUTTON_DOWN,
                            PIN_BUTTON_LEFT,
                            PIN_BUTTON_RIGHT,
                            PIN_BUTTON_SPARE}
{
    uint32_t currMs = millis();
    for (int i = 0; i < INPUT_NUM_PINS; i++) {
        pinMode(pins[i], INPUT_PULLUP);
        buttonStates[i] = {HIGH, currMs, 0, NULL};
    }
}

void Input_::begin(void)
{
    pinMode(PIN_ENCODER_A, INPUT_PULLUP);
    pinMode(PIN_ENCODER_B, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_A), encoderAInterrupt, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_B), encoderBInterrupt, CHANGE);
}

void Input_::loop(void)
{
    // Buttons
    uint32_t currMs = millis();
    for (int i = 0; i < INPUT_NUM_PINS; i++) {
        uint32_t diffMs = currMs - buttonStates[i].lastMs;
        int level = digitalRead(pins[i]);
        if (level != buttonStates[i].level) {
            if (diffMs > DEBOUNCE_TIME) {
                buttonStates[i].lastMs = currMs;
                buttonStates[i].level = level;
                buttonStates[i].repeatCount = 0;
                if (buttonStates[i].handler) {
                    buttonStates[i].handler(level == LOW ? INPUT_EVENT_PRESSED : INPUT_EVENT_RELEASED);
                }
            }
        } else if (buttonStates[i].level == LOW) {
            if (diffMs > (HELD_TIME + (buttonStates[i].repeatCount * REPEAT_TIME))) {
                buttonStates[i].repeatCount++;
                if (buttonStates[i].handler) {
                    buttonStates[i].handler(INPUT_EVENT_HELD);
                }
            }
        }
    }
    // Encoder
    unsigned int tmpIdx;
    int diff;
    noInterrupts();
    tmpIdx = encoderState.idx;
    interrupts();
    diff = tmpIdx - encoderState.oldIdx;
    encoderState.oldIdx = tmpIdx;
    if (diff != 0) {
        if (encoderState.handler) {
            encoderState.handler(diff);
        }
    }
}

void Input_::setButtonHandler(InputButton button, void (*handler)(InputEvent event))
{
    buttonStates[button].handler = handler;
}

void Input_::setEncoderHandler(void (*handler)(int diff))
{
    encoderState.handler = handler;
}

void Input_::encoderAInterrupt(void)
{
    int newLevel = digitalRead(PIN_ENCODER_A);
    // Ignore bounces
    if (newLevel == encoderState.levelA) {
        return;
    } else {
        encoderState.levelA = newLevel;
    }
    if (encoderState.levelA == encoderState.levelB) {
        encoderState.state++;
        if (encoderState.state == 4) {
            encoderState.state = 0;
            encoderState.idx++;
        }
    } else {
        encoderState.state--;
        if (encoderState.state == -4) {
            encoderState.state = 0;
            encoderState.idx--;
        }
    }
}

void Input_::encoderBInterrupt(void)
{
    int newLevel = digitalRead(PIN_ENCODER_B);
    // Ignore bounces
    if (newLevel == encoderState.levelB) {
        return;
    } else {
        encoderState.levelB = newLevel;
    }
    if (encoderState.levelA != encoderState.levelB) {
        encoderState.state++;
        if (encoderState.state == 4) {
            encoderState.state = 0;
            encoderState.idx++;
        }
    } else {
        encoderState.state--;
        if (encoderState.state == -4) {
            encoderState.state = 0;
            encoderState.idx--;
        }
    }
}

Input_ Input;
