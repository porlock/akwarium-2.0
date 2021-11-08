#pragma once
#include <stdint.h>

enum Pins : uint16_t
{
    eDigitalPinSoundOut = 6,
    eDigitalPinTempIn = 4,
    eDigitalPinJoySelect = 12,
    eAnalogPinJoyX = 0,
    eAnalogPinJoyY = 1
};

/*
#define DPIND_SOUNDOUT 6
#define DPIN_TEMPIN 4
#define DPIN_SELECT 12
#define APIN_JOY_X 0
#define APIN_JOY_Y 1
#define APIN_PHIN 6
#define DPIN_KOUT 2
#define DPIN_ZOUT 5
#define HEAT 3
*/

enum JoyStatus
{
    eUp,
    eDown,
    eLeft,
    eRight,
    eCenter,
    eSelect
};

enum ScreenType
{
    eScreenMain,
    eScreenSettings,
    eScreenPhCalibraton,
};
