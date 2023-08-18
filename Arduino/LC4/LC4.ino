#include "Control.hpp"
#include "Encoder.hpp"
#include "Input.hpp"
#include "Motor.hpp"
#include "Network.hpp"
#include "Rainbow.hpp"
#include "Screen.hpp"

void setup() {
    Serial.begin(115200);
    Control.begin();
    Encoder.begin();
    Input.begin();
    Motor.begin();
    Network.begin();
    //Rainbow.begin();
    Screen.begin();
}

void loop() {
    Input.loop();
    Motor.loop();
    Network.loop();
    //Rainbow.loop();
}
