#include <JoystickType.h>

JoystickType::JoystickType()
{
    status = JoyStatus::eCenter;
    prevStatus = JoyStatus::eCenter;
}

void JoystickType::readPins()
{
    horizontal = analogRead(Pins::eAnalogPinJoyX);
    vertical = analogRead(Pins::eAnalogPinJoyY);
    select = digitalRead(Pins::eDigitalPinJoySelect);
}

void JoystickType::beep()
{
    analogWrite(Pins::eDigitalPinSoundOut, 50);
    delay(100);
    digitalWrite(Pins::eDigitalPinSoundOut, LOW);
}

JoyStatus JoystickType::readState()
{
    readPins();
    if (!select)
    {
        status = JoyStatus::eSelect;
    }
    else if (horizontal > 700)
    {
        status = JoyStatus::eLeft;
    }
    else if (horizontal < 300)
    {
        status = JoyStatus::eRight;
    }
    else if (vertical > 700)
    {
        status = JoyStatus::eDown;
    }
    else if (vertical < 300)
    {
        status = JoyStatus::eUp;
    }
    else
    {
        status = JoyStatus::eCenter;
    }

    if (status == prevStatus)
    {
        return JoyStatus::eCenter;
    }
    prevStatus = status;
    beep();
    return status;
}
