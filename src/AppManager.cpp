#include "AppManager.h"

AppManager::AppManager(DisplayManager* dm, EventHandler* eh, HardwareManager* hwm) : _dm(dm), _eh(eh), _hwm(hwm){
    _tft = _dm->getDisplay();
    _offscreen = new TFT_eSprite(_tft);
    _offscreen->createSprite(_tft->width(), _tft->height());
    
    if (SPIFFS.exists(FONT_TIME_PATH)) {
        hasFontTime = true;
    }
    if (SPIFFS.exists(FONT_SIZE_16_PATH)) {
        hasFontSize16 = true;
    }
}

void AppManager::handleLoop(){
    RTC_Date now = _hwm->getCurrentTime();
    float v = _hwm->getVoltage();
    uint8_t percentage = _hwm->calcBatteryPercentage(v);

    String hh = now.hour < 10 ? "0" + String(now.hour) : String(now.hour);
    String mm = now.minute < 10 ? "0" + String(now.minute) : String(now.minute);
    String yyyy = String(now.year);
    String m = now.month < 10 ? "0" + String(now.month) : String(now.month);
    String d = now.day < 10 ? "0" + String(now.day) : String(now.day);
    String voltage = String(v, 2) + "V "+ String(percentage) + "%";
    if (_hwm->isBatteryCharging()) {
        voltage = voltage + " CHG";
    } else {
        voltage = voltage + "    ";
    }

    if (hasFontTime) {
        _offscreen->loadFont(FONT_TIME);
    }
    _offscreen->setTextColor(TFT_WHITE, TFT_BLACK);
    _offscreen->drawString(hh + ":" + mm, 9, 20);
    if (hasFontTime) {
        _offscreen->unloadFont();
    }

    if (hasFontSize16) {
        _offscreen->loadFont(FONT_SIZE_16);
    }
    _offscreen->setTextColor(TFT_DARKGREY, TFT_BLACK);
    _offscreen->drawString(yyyy + "-" + m + "-" + d, 31, 2);
    _offscreen->setTextColor(TFT_DARKGREY, TFT_BLACK);
    _offscreen->drawString(voltage, 0, 64);
    if (hasFontSize16) {
        _offscreen->unloadFont();
    }
    
    if(_offscreen != nullptr) {
        _offscreen->pushSprite(0, 0);
    }
}

void AppManager::checkEvents(){
    if(_eh->isButtonJustReleased()){
        switchToNextMode();
    }
}

void AppManager::switchToNextMode(){
    
}