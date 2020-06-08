#ifndef APPMANAGER_H
#define APPMANAGER_H

#include <Arduino.h>
#include "EventHandler.h"
#include "DisplayManager.h"
#include "HardwareManager.h"

#define FONT_TIME "SourceCodePro-Regular-48"
#define FONT_TIME_PATH "/SourceCodePro-Regular-48.vlw"
#define FONT_SIZE_16 "SourceCodePro-Regular-16"
#define FONT_SIZE_16_PATH "/SourceCodePro-Regular-16.vlw"

#define NUM_MODES 2
enum MODE { SCREEN_TIME = 0, SCREEN_OTA = 1 };

class AppManager{
    public:
        AppManager(DisplayManager* dm, EventHandler* eh, HardwareManager* hwm);
        void checkEvents();
        void handleLoop();
    protected:
        void switchToNextMode();
    private:
        DisplayManager* _dm = nullptr;
        EventHandler* _eh = nullptr;
        HardwareManager* _hwm = nullptr;
        TFT_eSPI* _tft = nullptr;
        TFT_eSprite* _offscreen = nullptr;
        bool hasFontTime = false;
        bool hasFontSize16 = false;
        uint8_t mode = SCREEN_TIME;

        void showTimeScreen();
        void showOTAScreen();
};

#endif //APPMANAGER_H