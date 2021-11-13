#pragma once
#include <stdint.h>
#include <ScreenManager.h>
#include <screen.h>
#include <GlobalEnums.h>
#include <SettingsType.h>

class PhCalibrationScreen
{
private:
    SettingsType &m_settings;
    uint32_t m_nextUpdateRead;
    uint32_t m_nextUpdateJoystick;
    CalibrationPhase currentCalibrationPhase;

public:
    PhCalibrationScreen(SettingsType&);
    void render();
    void control(JoyStatus);
};