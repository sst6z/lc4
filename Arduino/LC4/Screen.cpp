#include "Screen.hpp"

#include <SPI.h>

#define NUM_PIXELS      144
#define NUM_BEAMS       4
#define PIXELS_PER_BEAM (NUM_PIXELS / NUM_BEAMS)

Screen_::Screen_(void)
    : strip(NUM_PIXELS, DOTSTAR_BGR), numPixels {numPixels}
{
    enabled = false;
    swapOnSync = false;
    pActiveBuffer = &buffer0;
    pNextBuffer = &buffer1;
    pDirtyBuffer = &buffer2;
}

void Screen_::begin(void)
{
    strip.begin();
    strip.show();
}

void Screen_::setPixel(int x, int y, uint32_t color)
{
    (*pDirtyBuffer)[y][x] = color;
}

void Screen_::swap(void)
{
    noInterrupts();
    auto tmp = pDirtyBuffer;
    pDirtyBuffer = pNextBuffer;
    pNextBuffer = tmp;
    swapOnSync = true;
    interrupts();
}

void Screen_::sync(void)
{
    if (swapOnSync) {
        noInterrupts();
        auto tmp = pActiveBuffer;
        pActiveBuffer = pNextBuffer;
        pNextBuffer = tmp;
        swapOnSync = false;
        interrupts();
    }
}

void Screen_::showLine(int y)
{
    if (enabled) {
        if (y >= SCREEN_WIDTH) {
            return;
        }
        int offset = 1;
        for (int b = 1; b <= NUM_BEAMS; b++) {
            for (int x = 0; x < PIXELS_PER_BEAM; x++) {
                strip.setPixelColor((PIXELS_PER_BEAM * b - 1) - x, Adafruit_DotStar::gamma32((*pActiveBuffer)[y][x * 2 + offset]));
            }
            y += (SCREEN_WIDTH / NUM_BEAMS);
            y %= SCREEN_WIDTH;
            offset = offset == 0 ? 1 : 0;
        }
        strip.show();
    }
}

void Screen_::enable(bool enable)
{
    enabled = enable;
    noInterrupts();
    strip.clear();
    strip.show();
    interrupts();
}

Screen_ Screen;
