#pragma once
#include <Arduino.h>
#include <ScreenManager.h>
#include <screen.h>
#include <GlobalEnums.h>
#include <SettingsType.h>
#include <probing.h>


class PhCalibrationScreen
{
private:
    SettingsType &m_settings;
    uint32_t m_nextUpdateRead;
    uint32_t m_nextUpdateJoystick;
    CalibrationPhase currentCalibrationPhase;
    uint16_t m_constLoopsCounter;
    UF16x3 voltage;
    UF16x3 voltagePrev;
    bool isConstant(uint16_t loops = 500);

public:
    PhCalibrationScreen(SettingsType &);
    void render();
    void control(JoyStatus);
};