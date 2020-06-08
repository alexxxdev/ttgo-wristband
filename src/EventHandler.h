#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <Arduino.h>
#include "pins.h"

class EventHandler{
    public:
        EventHandler();
        void poll();
        bool isButtonPressed();
        bool timeoutForSleepReached();
        bool isButtonJustReleased();
    private:
        // go to sleep after 10 seconds (if allowed)
        const uint16_t _MAX_TIMEOUT = 5000;
        uint32_t _lastEventAt = 0;
        bool isPressed = false;
        bool lastPressed = false;
};

#endif //EVENTHANDLER_H