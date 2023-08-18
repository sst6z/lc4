#include "Encoder.hpp"

#include "Screen.hpp"

#define PIN_ENCODER_A   2
#define PIN_ENCODER_B   3
#define PIN_SYNC        21

// Intentionally an unsigned value because it relies on defined wrap around behavior.
volatile uint32_t Encoder_::lastUsEncoder;
volatile uint32_t Encoder_::lastUsSync;
volatile float Encoder_::rpm;
volatile uint32_t Encoder_::y;
volatile bool Encoder_::intBTriggered;

void Encoder_::begin(void)
{
    pinMode(PIN_ENCODER_A, INPUT_PULLUP);
    pinMode(PIN_ENCODER_B, INPUT_PULLUP);
    pinMode(PIN_SYNC, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_A), interruptEncoderA, RISING);
    attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_B), interruptEncoderB, RISING);
    attachInterrupt(digitalPinToInterrupt(PIN_SYNC), interruptSync, RISING);
    lastUsEncoder = micros();
    lastUsSync = micros();
    rpm = 0.;
    y = 0;
    intBTriggered = false;
}

float Encoder_::getRpm(void)
{
    float tmpRpm;
    noInterrupts();
    tmpRpm = rpm;
    interrupts();
    return (tmpRpm);
}

void Encoder_::interruptEncoderA(void)
{
    uint32_t currUs = micros();
    uint32_t elapsedUs = currUs - lastUsEncoder;
    if (elapsedUs < 400) {
        // Debounce
        return;
    } else {
        lastUsEncoder = currUs;
    }

    Screen.showLine(y);
    y++;
}

void Encoder_::interruptEncoderB(void)
{
    intBTriggered = true;
}

void Encoder_::interruptSync(void)
{
    uint32_t currUs = micros();
    uint32_t elapsedUs = currUs - lastUsSync;
    if (elapsedUs < 100000) {
        // Debounce
        return;
    }
    lastUsSync = currUs;
    rpm = 1000000 * 60 / (float)elapsedUs;
    y = 0;
    Screen.sync();
}

Encoder_ Encoder;
