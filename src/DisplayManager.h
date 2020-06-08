#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <esp32-hal-ledc.h>

class DisplayManager {
    public:
        DisplayManager();
        void setFrameRate(uint8_t rate);
        bool nextFrame();
        bool everyXFrames(uint8_t frames);
        TFT_eSPI* getDisplay();
        void commenceSleep();
    private:
        TFT_eSPI* _tft = new TFT_eSPI();
        uint8_t frameRate;
        uint16_t frameCount;
        uint8_t eachFrameMillis;
        long lastFrameStart;
        long nextFrameStart;
        bool post_render;
        uint8_t lastFrameDurationMs;
};

#endif //DISPLAYMANAGER_H