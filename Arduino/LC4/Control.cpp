#include "Control.hpp"

#include "Motor.hpp"
#include "Screen.hpp"

#define MOTOR_DEFAULT_SPEED 255

bool Control_::motorOn = false;
int Control_::speed = MOTOR_DEFAULT_SPEED;

void Control_::begin(void)
{
    Input.setButtonHandler(INPUT_BUTTON_ENCODER, buttonHandlerEncoder);
    Input.setEncoderHandler(encoderHandler);
}

void Control_::buttonHandlerEncoder(InputEvent event)
{
    if (event == INPUT_EVENT_PRESSED) {
        if (motorOn) {
            Motor.setSpeed(0);
            Screen.enable(false);
        } else {
            Motor.setSpeed((uint8_t)speed);
            Screen.enable(true);
        }
        motorOn = !(motorOn);
    }
}

void Control_::encoderHandler(int diff)
{
    if (motorOn) {
        speed += (diff * 10);
        if (speed > 255) speed = 255;
        if (speed < 0) speed = 0;
        Motor.setSpeed((uint8_t)speed);
    }
}

Control_ Control;
