#pragma once
#include <Arduino.h>
#include "FixedPoint.h"

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

    uint16_t readV( uint16_t delay=50, uint16_t loops=30);
}