#pragma once
#include "FixedPoint.h"

struct SettingsType
{
    struct PhSettingsType
    {
        UF16x2 up;
        UF16x2 down;
        uint16_t interval;
        uint16_t onTime;
    }phSettings;

    struct TempSettingsType
    {
        UF16x2 up;
        UF16x2 down;
    }tempSettings;

    struct PhCalibrationSettingsType
    {
        UF16x2 ph7V;
        UF16x2 ph4V;
        UF16x2 phFactor;
    }phCalibrationSettings;

    struct ScreenSettingsType
    {
        uint16_t barLenght;        
    }screenSettings;

};

extern SettingsType defaultSettings;
extern SettingsType settings;


