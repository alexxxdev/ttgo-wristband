#ifndef UTIL_H
#define UTIL_H
#include <TFT_eSPI.h>
#include <pins.h>

void setMinCPUSpeed();
void setMaxCPUSpeed();

void setupWatch();
void initGPIOs();
void initRTC();
void initSensor();
void initScreen();

void disableRadios();
void deepSleep();

void clearScreen();
void drawDateRow();
void drawTime();
void drawInfoRow();
void drawCompass();

bool isCharging();
bool isButtonDown();

#endif