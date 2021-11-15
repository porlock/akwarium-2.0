#include <PhCalibrationScreen.h>

PhCalibrationScreen::PhCalibrationScreen(SettingsType &_settings)
    : m_settings(_settings),
      currentCalibrationPhase(CalibrationPhase::eCalibrationStart),
      m_constLoopsCounter(0),
      voltage(0),
      voltagePrev(0) {}

void PhCalibrationScreen::control(JoyStatus joyState)
{
    if (millis() - m_nextUpdateJoystick > 100)
    {
        m_nextUpdateJoystick = millis();
        if (joyState == JoyStatus::eSelect)
            setNextScreen(ScreenType::eScreenSettings);
        else
        {             
            if (currentCalibrationPhase == CalibrationPhase::eCalibrationPh7Read && (voltage < 3.1) && (voltage > 2.9))
            {
                currentCalibrationPhase = CalibrationPhase::eCalibrationPh7Stable;
                m_constLoopsCounter = 0;
            }
            else if (currentCalibrationPhase == CalibrationPhase::eCalibrationPh7Stable && isConstant())
            {
                currentCalibrationPhase = CalibrationPhase::eCalibrationPh4Read;
            }
            else if (currentCalibrationPhase == CalibrationPhase::eCalibrationPh4Read && (voltage < 3.50) and (voltage > 3.30))
            {
                currentCalibrationPhase = CalibrationPhase::eCalibrationPh7Stable;
                m_constLoopsCounter = 0;
            }
            else if (currentCalibrationPhase == CalibrationPhase::eCalibrationPh7Stable && isConstant())
            {
                currentCalibrationPhase = CalibrationPhase::eCalibrationEnd;
            }

            voltage = probing::readV();
        }
    }
}

bool PhCalibrationScreen::isConstant(uint16_t loops)
{
    if (abs(voltage - voltagePrev) < 0.01f)
    {
        if (m_constLoopsCounter >= loops)
        {
            return true;
        }
        m_constLoopsCounter++;
    }
    else
        m_constLoopsCounter = 0;
    return false;
}

void PhCalibrationScreen::render()
{
    if (millis() - m_nextUpdateRead > 500)
    {
        m_nextUpdateRead = millis();
        if (currentCalibrationPhase == CalibrationPhase::eCalibrationEnd)
            screen::showSave();
        else
            screen::showCalibration(currentCalibrationPhase);
    }
}
