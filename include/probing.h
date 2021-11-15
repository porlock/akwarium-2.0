#pragma once
#include <Arduino.h>
#include "FixedPoint.h"
#include "SMA.h"
#include "GlobalEnums.h"
#include "SettingsType.h"

namespace probing
{
    struct ReadingsType
    {
        UF16x2 ph;
        UF16x2 temp;
        bool waterLevel;
    };
    extern ReadingsType readings;

    void readPH();

    void readTemp();

    void readWaterLevel();

    void runLoop();

    UF16x3 readV( uint16_t delay=5, uint16_t loops=10);
}