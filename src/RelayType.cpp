#include <RelayType.h>

RelayType::RelayType(uint8_t _outPin, bool invertedLogic = false): go(false), clock(0)
{
    outPin = _outPin;
    inverted = invertedLogic;
};

void RelayType::initPin()
{
    pinMode(outPin, OUTPUT);
}

void RelayType::setOn()
{
    if (inverted)
        digitalWrite(outPin, LOW);
    else
        digitalWrite(outPin, HIGH);
};

void RelayType::setClockOn(uint32_t onTimeMilis, uint32_t intervalMilis)
{
    if (millis() - clock > intervalMilis)
    {
        clock = millis();
        go = true;
    }

    if (go)
    {
        if (millis() - clock < onTimeMilis)
        {
            void setOn();
        }
        else
        {
            setOff();
            go = false;
        }
    }
};

void RelayType::setOff()
{
    if (inverted)
        digitalWrite(outPin, HIGH);
    else
        digitalWrite(outPin, LOW);
}
