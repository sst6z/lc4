#include "Motor.hpp"

#define PIN_IN1         23
#define PIN_IN2         22
#define PIN_PWM         18

void Motor_::begin(void)
{
    pinMode(PIN_IN1, OUTPUT);
    pinMode(PIN_IN2, OUTPUT);
    pinMode(PIN_PWM, OUTPUT);
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, HIGH);
    analogWrite(PIN_PWM, 0);
    requestedSpeed = 0;
    currentSpeed = 0;
    lastMs = millis();
}

void Motor_::loop(void)
{
    if (requestedSpeed == currentSpeed) {
        return;
    }

    uint32_t currMs = millis();
    if ((currMs - lastMs) < 10) {
        return;
    } else {
        lastMs = currMs;
    }

    int adjust;
    adjust = (requestedSpeed > currentSpeed) ? 1 : -1;
    currentSpeed += adjust;
    if (currentSpeed == 255) {
        digitalWrite(PIN_PWM, HIGH);
    } else {
        analogWrite(PIN_PWM, currentSpeed);
    }
}

void Motor_::setSpeed(uint8_t speed)
{
    requestedSpeed = speed;
}

Motor_ Motor;
