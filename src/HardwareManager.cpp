#include "HardwareManager.h"

HardwareManager::HardwareManager() {
    initRL1();
}

void HardwareManager::initRL1() {
    if(_runLevel != RL0){
        return;
    }

    setCpuFrequencyMhz(80);

    initClock();
    setupADC();
    setupBattery();
    initSPIFFS();
    
    btStop();
    _runLevel = RL1;
}

void HardwareManager::initRL2() {
    if(_runLevel != RL1){
        return;
    }

    setCpuFrequencyMhz(160);

    _runLevel = RL2;
}

void HardwareManager::terminateRL2() {
    if(_runLevel != RL2){
        log_w("Can't terminate RunLevel 2 (%d)", _runLevel);
        return;
    }

    log_i("Terminating RunLevel 2, switching to RunLevel 1");
    _runLevel = RL1;
}

void HardwareManager::initClock() {
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
    Wire.setClock(400000);
    _rtc.begin(Wire);
    _rtc.check();
}

void HardwareManager::setupADC() {
    esp_adc_cal_characteristics_t adc_chars;
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(
            (adc_unit_t)ADC_UNIT_1,
            (adc_atten_t)ADC1_CHANNEL_6,
            (adc_bits_width_t)ADC_WIDTH_BIT_12,
            1100,
            &adc_chars);
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF){
        _vref = adc_chars.vref;
    }
}

void HardwareManager::setupBattery() {
    pinMode(CHARGE_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);
}

void HardwareManager::initSPIFFS(){
    if(!SPIFFS.begin(true)){
        Serial.println("SPIFFS Mount Failed");
    }
}

RTC_Date HardwareManager::getCurrentTime() {
    return _rtc.getDateTime();
}

bool HardwareManager::switchToRunLevel(RunLevel requestedRL) {
    while(requestedRL != _runLevel) {
        if(requestedRL > _runLevel) {
            switch(_runLevel) {
                case RL1:
                    initRL2();
                    break;
                default:
                    log_e("non-reachable runlevel status reached, device will go up in flames now.");
                    return false;
            }
        } else {
            switch(_runLevel) {
                case RL2:
                    terminateRL2();
                    break;
                case RL1:
                    commenceSleep();
                    break;
                default:
                    log_e("non-reachable runlevel status reached, device will go up in flames now.");
                    return false;
            }
        }
    }

    return true;
}

void HardwareManager::commenceSleep(){
    _runLevel = RL0;
    log_i("Going into sleep mode");
    delay(20);

    rtcSleep();
    esp_sleep_enable_ext1_wakeup(GPIO_SEL_33, ESP_EXT1_WAKEUP_ANY_HIGH);
    esp_deep_sleep_disable_rom_logging();
    esp_deep_sleep_start();
}

void HardwareManager::rtcSleep(){
    _rtc.clearTimer();
    _rtc.resetAlarm();
    _rtc.disableAlarm();
    _rtc.disableCLK();
    _rtc.disableTimer();
}

float HardwareManager::getVoltage() {
    uint16_t v = analogRead(BATT_ADC_PIN);

    float battery_voltage = ((float)v / 4095.0) * 2.0 * 3.3 * (_vref / 1000.0);

    return battery_voltage;
}

uint8_t HardwareManager::calcBatteryPercentage(float volts) {
    float percentage = (volts - _BATTERY_MIN_V) * 100 / (_BATTERY_MAX_V - _BATTERY_MIN_V);
    if (percentage > 100){
        percentage = 100;
    }
    if (percentage < 0){
        percentage = 0;
    }
    return percentage;
}

bool HardwareManager::isBatteryCharging() {
    return !digitalRead(CHARGE_PIN);
}

void HardwareManager::updateChargeLED() {
    digitalWrite(LED_PIN, isBatteryCharging());
}