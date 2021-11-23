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
    CalibrationPhase m_currentCalibrationPhase;
    uint16_t m_constLoopsCounter;
    F32x3 m_voltage;
    F32x3 m_voltagePrev;
    bool isConstant(uint16_t loops = 500);
    bool m_blink;

public:
    PhCalibrationScreen(SettingsType &);
    void render();
    void control(JoyStatus);
};