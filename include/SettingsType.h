#pragma once
#include <FixedPoint.h>

struct SettingsType
{
    struct PhSettingsType
    {
        F16x3 up = 6.7f;
        F16x3 down = 6.5f;
        uint16_t interval = 30;
        uint16_t onTime = 3;
    } phSettings;

    struct TempSettingsType
    {
        F16x3 up = 27.0f;
        F16x3 down = 25.0f;
    } tempSettings;

    struct PhCalibrationSettingsType
    {
        F16x3 ph7V = 2.93f;
        F16x3 ph4V = 3.37f;
        F16x3 phFactor = 0.15f;
    } phCalibrationSettings;

    struct ScreenSettingsType
    {
        uint16_t barLenght = 15;
    } screenSettings;
    float vRef = 4.49f;
    // musi byc zadeklarowany ostatni
    uint32_t eepromCheck = 3768890503;    
};

extern SettingsType defaultSettings;
extern SettingsType settings;
