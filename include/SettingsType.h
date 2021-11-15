#pragma once
#include <FixedPoint.h>

struct SettingsType
{
    struct PhSettingsType
    {
        UF16x2 up = 6.7f;
        UF16x2 down = 6.5f;
        uint16_t interval = 30;
        uint16_t onTime = 3;
    } phSettings;

    struct TempSettingsType
    {
        UF16x2 up = 27.0f;
        UF16x2 down = 25.0f;
    } tempSettings;

    struct PhCalibrationSettingsType
    {
        UF16x2 ph7V = 0.0f;
        UF16x2 ph4V = 0.0f;
        UF16x2 phFactor = 0.0f;
    } phCalibrationSettings;

    struct ScreenSettingsType
    {
        uint16_t barLenght = 15;
    } screenSettings;

    uint32_t eepromCheck = 3768890503;
};

extern SettingsType defaultSettings;
extern SettingsType settings;
