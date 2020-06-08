#include "EventHandler.h"

EventHandler::EventHandler() {
    //! Must be set to pull-up output mode in order to wake up in deep sleep mode
    pinMode(TP_PWR_PIN, PULLUP);
    digitalWrite(TP_PWR_PIN, HIGH);

    pinMode(TP_PIN_PIN, INPUT);

    _lastEventAt = millis();
}

void EventHandler::poll(){
    lastPressed = isPressed;
    isPressed = gpio_get_level(GPIO_NUM_33);
    if(isButtonJustPressed()){
        _lastEventAt = millis();
    }
}

bool EventHandler::isButtonPressed() {
    return isPressed;
}

bool EventHandler::timeoutForSleepReached() {
    return _lastEventAt + _MAX_TIMEOUT <= millis();
}

bool EventHandler::isButtonJustReleased() {
    return lastPressed && !isPressed;
}

bool EventHandler::isButtonJustPressed() {
    return !lastPressed && isPressed;
}