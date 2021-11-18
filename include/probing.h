#pragma once
#include <Arduino.h>
#include "FixedPoint.h"
#include "SMA.h"
#include "GlobalEnums.h"
#include "SettingsType.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include "screen.h"

namespace probing
{
    struct ReadingsType
    {
        F16x3 ph;
        F16x3 temp;
        F16x3 phVoltage;
        bool waterLevel;
    };
    extern ReadingsType readings;

    void readPH();

    void readTemp();

    void readWaterLevel();

    void runLoop();

    void initTemperatureSensor();

    F16x3 readV( uint16_t delay=5, uint16_t loops=10);
}