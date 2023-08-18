#include "Rainbow.hpp"

#include "Screen.hpp"

void Rainbow_::begin(void)
{
    offset = 0;
}

void Rainbow_::loop(void)
{
    uint16_t hue;
    uint32_t color;
    for (int y = 0; y < SCREEN_WIDTH; y++) {
        hue = y * (UINT16_MAX / SCREEN_WIDTH) + offset;
        for (int x = 0; x < SCREEN_HEIGHT; x++) {
            color = Adafruit_DotStar::ColorHSV(hue + (x * (UINT16_MAX / SCREEN_HEIGHT)));
            Screen.setPixel(x, y, color);
        }
    }
    offset += 200;
    Screen.swap();
}

Rainbow_ Rainbow;
