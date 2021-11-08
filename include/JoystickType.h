#pragma once
#include <GlobalEnums.h>
#include <Arduino.h>


class JoystickType
{
    private:
        uint16_t horizontal, vertical, select;
        JoyStatus status, prevStatus;
        void readPins();
        void beep();
   
    public:
        JoyStatus readState();
        JoystickType();
};