
#include "HardwareManager.h"
#include "DisplayManager.h"
#include "EventHandler.h"
#include "AppManager.h"

DisplayManager* _dm = nullptr;
HardwareManager* _hwm = nullptr;
EventHandler* _eh = nullptr;
AppManager* _am = nullptr;

void setup() {
  Serial.begin(115200);
  _hwm = new HardwareManager();
  _dm = new DisplayManager();
  _eh = new EventHandler();
  _am = new AppManager(_dm, _eh, _hwm);

}

void loop() {
  if(!_dm->nextFrame()){
    return;
  }

  _eh->poll();

  _am->checkEvents();

  if (_eh->timeoutForSleepReached() && !_eh->isButtonPressed() && !_hwm->isBatteryCharging()) {
        _dm->commenceSleep();
        _hwm->switchToRunLevel(RL0);
  }

  _hwm->updateChargeLED();

  _am->handleLoop();
}