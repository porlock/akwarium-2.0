#pragma once
#include <stdint.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include "FixedPoint.h"

namespace screen
{
    
    struct IconsIndicatorsType{
        bool refill;
        bool hci;
        bool heater;
    };

    extern IconsIndicatorsType iconsIndicator;

    void initializeScreen();
    void clearScreen();
    void showLogo();
    void showReadings(uint16_t , const char* , UF16x2 , bool = false);
    void showSettings(uint16_t , uint16_t , const char* , UF16x2 );
    void showClickOption(const char* , uint16_t = 1, uint16_t = 0);
    void showSave();
}