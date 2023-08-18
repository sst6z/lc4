#ifndef _NETWORK_h
#define _NETWORK_h

#include "Arduino.h"

#include <SPI.h>
#include <NativeEthernet.h>

class Network_ {
    public:
        Network_(void);
        void begin(void);
        void loop(void);
    private:
        uint8_t mac[6];
        void getMac(uint8_t* pMac);
        EthernetServer server;
        bool clientConnected;
        EthernetClient client;
        int pixelCount;
        uint8_t incompletePixel[3];
        int incompleteCount;
};

extern Network_ Network;

#endif
