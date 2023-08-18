#ifndef _SCREEN_h
#define _SCREEN_h

#include "Arduino.h"
#include <Adafruit_DotStar.h>

#define SCREEN_HEIGHT   72
#define SCREEN_WIDTH    374

class Screen_ {
    public:
        Screen_(void);
        void begin(void);
        void setPixel(int x, int y, uint32_t color);
        void swap(void);
        void sync(void);
        void showLine(int y);
        void enable(bool enable);
    private:
        Adafruit_DotStar strip;
        uint16_t numPixels;
        uint32_t buffer0[SCREEN_WIDTH][SCREEN_HEIGHT];
        uint32_t buffer1[SCREEN_WIDTH][SCREEN_HEIGHT];
        uint32_t buffer2[SCREEN_WIDTH][SCREEN_HEIGHT];
        uint32_t (*pActiveBuffer)[SCREEN_WIDTH][SCREEN_HEIGHT];
        uint32_t (*pNextBuffer)[SCREEN_WIDTH][SCREEN_HEIGHT];
        uint32_t (*pDirtyBuffer)[SCREEN_WIDTH][SCREEN_HEIGHT];
        bool enabled;
        volatile bool swapOnSync;
};

extern Screen_ Screen; // Use EXTMEM if it doesn't fit into internal RAM.

#endif
