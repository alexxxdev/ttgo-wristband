#ifndef HARDWAREMANAGER_H
#define HARDWAREMANAGER_H

#include "pins.h"
#include <esp32-hal-cpu.h>
#include <esp_adc_cal.h>
#include <WiFi.h>
#include <Arduino.h>
#include <pcf8563.h>
#include "SPIFFS.h"

enum RunLevel : uint8_t {
    RL0 =0, // shutdown
    RL1 =1, // display and clock, spiffs and battery
    RL2 =2  // all the rest
};

class HardwareManager {
    public:
        HardwareManager();
        void initRL1();
        void initRL2();
        void terminateRL2();
        RTC_Date getCurrentTime();
        bool switchToRunLevel(RunLevel requestedRL); // switches to the requested runlevel (going down as well).
        float getVoltage();
        uint8_t calcBatteryPercentage(float volts);
        bool isBatteryCharging();
        void updateChargeLED();
    protected:
        void initClock();
        void setupADC();
        void setupBattery();
        void initSPIFFS();
        void rtcSleep();
    private:
        RunLevel _runLevel = RL0;
        PCF8563_Class _rtc;
        uint32_t _vref = 1100;
        const float _BATTERY_MIN_V = 3.2;
        const float _BATTERY_MAX_V = 4.1;

        void commenceSleep();
};

#endif //HARDWAREMANAGER_H