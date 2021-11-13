#include <PhCalibrationScreen.h>

PhCalibrationScreen::PhCalibrationScreen(SettingsType &_settings)
    : m_settings(_settings),
      currentCalibrationPhase(CalibrationPhase::eCalibrationStart) {}

void PhCalibrationScreen::control(JoyStatus joyState)
{
    if (millis() - m_nextUpdateJoystick > 100)
    {
        m_nextUpdateJoystick = millis();
        if (joyState == JoyStatus::eSelect)
            setNextScreen(ScreenType::eScreenSettings);
    }
}

void PhCalibrationScreen::render()
{
    if (millis() - m_nextUpdateRead > 500)
    {
        m_nextUpdateRead = millis();
        screen::showCalibration(currentCalibrationPhase);
    }
}
