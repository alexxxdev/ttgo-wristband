#include "DisplayManager.h"

DisplayManager::DisplayManager() {
    setFrameRate(60);
    frameCount = 0;
    nextFrameStart = 0;
    lastFrameStart = 0;
    post_render = false;

    _tft->init();
    _tft->setRotation(1);
    _tft->setSwapBytes(true);
    _tft->fillScreen(TFT_BLACK);
    ledcSetup(0, 5000, 8);
    ledcAttachPin(TFT_BL, 0);
    ledcWrite(0, 185);
}

void DisplayManager::setFrameRate(uint8_t rate) {
    frameRate = rate;
    eachFrameMillis = 1000 / rate;
}

TFT_eSPI* DisplayManager::getDisplay() {
    return _tft;
}

bool DisplayManager::everyXFrames(uint8_t frames) {
    return frameCount % frames == 0;
}

bool DisplayManager::nextFrame() {
    long now = millis();
    //uint8_t remaining;

    // post render
    if (post_render) {
        lastFrameDurationMs = now - lastFrameStart;
        frameCount++;
        post_render = false;
    }

    // if it's not time for the next frame yet
    if (now < nextFrameStart) {
        // remaining = nextFrameStart - now;
        // if we have more than 1ms to spare, lets sleep
        // we should be woken up by timer0 every 1ms, so this should be ok
        //if (remaining > 1)
        //    idle();
        return false;
    }

    // pre-render

    // technically next frame should be last frame + each frame but if we're
    // running a slow render we would constantly be behind the clock
    // keep an eye on this and see how it works.  If it works well the
    // lastFrameStart variable could be eliminated completely
    nextFrameStart = now + eachFrameMillis;
    lastFrameStart = now;
    post_render = true;

    return post_render;
}

void DisplayManager::commenceSleep() {
    _tft->fillScreen(TFT_BLACK);
    _tft->writecommand(ST7735_SWRESET);
    delay(100);
    _tft->writecommand(ST7735_SLPIN);
    delay(150);
    _tft->writecommand(ST7735_DISPOFF);
}