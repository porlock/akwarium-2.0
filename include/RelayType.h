#pragma once
#include <Arduino.h>

class RelayType
{

private:
    bool go;
    uint32_t clock;
    uint8_t outPin;
    uint8_t period;
    bool inverted;
 
public:
    RelayType(uint8_t _outPin, bool invertedLogic = false);
    void initPin();
    void setOn();
    void setClockOn(uint32_t onTimeMilis, uint32_t intervalMilis);
    void setOff();
};
