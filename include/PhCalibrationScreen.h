#pragma once
#include <stdint.h>
#include <ScreenManager.h>
#include <screen.h>
#include <GlobalEnums.h>
#include <SettingsType.h>
#include <probing.h>
#include "QuickStats.h"

class PhCalibrationScreen
{
private:
    SettingsType &m_settings;
    uint32_t m_nextUpdateRead;
    uint32_t m_nextUpdateJoystick;
    CalibrationPhase currentCalibrationPhase;
    uint16_t m_constLoopsCounter;
    uint16_t voltage;
    uint16_t voltagePrev;
    bool isConstant(uint16_t loops = 500);

public:
    PhCalibrationScreen(SettingsType &);
    void render();
    void control(JoyStatus);
};