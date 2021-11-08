#pragma once
#include <stdint.h>
#include <ScreenManager.h>
#include <screen.h>

class PhCalibrationScreen
{
    uint16_t i;

public:
    void startPhCalibration();
    PhCalibrationScreen();
};