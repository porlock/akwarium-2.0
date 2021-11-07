#pragma once
#include "FixedPoint.h"

struct SettingsType
{
    struct PhSettingsType
    {
        UF16x2 up = 6.8f;
        UF16x2 down = 6.5f;
        uint16_t interval = 10;
        uint16_t onTime = 2;
    }phSettings;

    struct TempSettingsType
    {
        UF16x2 up = 27.0f;
        UF16x2 down = 25.5f;
    }tempSettings;

    struct PhCalibrationSettingsType
    {
        UF16x2 ph7V=0;
        UF16x2 ph4V=0;
        UF16x2 phFactor=0;
    }phCalibrationSettings;

    struct ScreenSettingsType
    {
        uint16_t barLenght=13;        
    }screenSettings;

} settings;

//extern SettingsType settings;


/*= {
    {6.7f, 6.5f, 300, 3},
    {27.0f,25.5f},
    {0.00f,0.00f,0.00f},
    {15}
}*/;