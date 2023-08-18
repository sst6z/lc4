#include "Network.hpp"

#include "Screen.hpp"
#include <SPI.h>

#define PORT    23

Network_::Network_(void)
    : server(PORT)
{
}

void Network_::begin(void)
{
    IPAddress ip(10, 0, 0, 42);
    getMac(mac);
    Ethernet.begin(mac, ip);
    server.begin();
    clientConnected = false;
}

void Network_::loop(void)
{
    if (!clientConnected) {
        EthernetClient newClient = server.accept();
        if (newClient) {
            client = newClient;
            clientConnected = true;
            pixelCount = 0;
            incompleteCount = 0;
        }
    }

    if (clientConnected) {
        if (client.available()) {
            uint8_t buf[64];
            // Fill beginning of buffer with last incomplete pixel.
            for (int i = 0; i < incompleteCount; i++) {
                buf[i] = incompletePixel[i];
            }
            int count = client.read(&buf[incompleteCount], sizeof(buf) - incompleteCount);
            if (count > 0) {
                count += incompleteCount;
                int i = 0;
                // Only show complete pixels.
                while (i < (count - count % 3)) {
                    uint32_t pixel = (uint32_t)buf[i + 0] << 16 |
                                     (uint32_t)buf[i + 1] <<  8 |
                                     (uint32_t)buf[i + 2] <<  0;
                    i += 3;
                    Screen.setPixel(pixelCount % SCREEN_HEIGHT, pixelCount / SCREEN_HEIGHT, pixel);
                    pixelCount++;
                    if (pixelCount >= SCREEN_HEIGHT * SCREEN_WIDTH) {
                        Screen.swap();
                        pixelCount = 0;
                    }
                }
                // Save incomplete pixel.
                incompleteCount = 0;
                while (i < count) {
                    incompletePixel[incompleteCount] = buf[i];
                    incompleteCount++;
                    i++;
                }
            }
        }
    }

    if (clientConnected && !client.connected()) {
        client.stop();
        clientConnected = false;
    }
}

void Network_::getMac(uint8_t* pMac) {
    for (int i = 0; i < 2; i++) pMac[i]=(HW_OCOTP_MAC1 >> ((1 - i) * 8)) & 0xFF;
    for (int i = 0; i < 4; i++) pMac[i + 2]=(HW_OCOTP_MAC0 >> ((3 - i) * 8)) & 0xFF;
}

Network_ Network;
