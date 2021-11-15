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
            voltage = probing::readV();

            if (currentCalibrationPhase == CalibrationPhase::eCalibrationPh7Read && (voltage < 3.1) && (voltage > 2.9))
            {
                currentCalibrationPhase = CalibrationPhase::eCalibrationPh7Stable;
                m_constLoopsCounter = 0;
            }
            else if (currentCalibrationPhase == CalibrationPhase::eCalibrationPh7Stable && isConstant())
            {
                currentCalibrationPhase = CalibrationPhase::eCalibrationPh4Read;
                m_settings.phCalibrationSettings.ph7V = voltage.as_float();
            }
            else if (currentCalibrationPhase == CalibrationPhase::eCalibrationPh4Read && (voltage < 3.50) and (voltage > 3.30))
            {
                currentCalibrationPhase = CalibrationPhase::eCalibrationPh7Stable;
                m_constLoopsCounter = 0;
            }
            else if (currentCalibrationPhase == CalibrationPhase::eCalibrationPh4Stable && isConstant())
            {
                    /*   
                    if (voltage >= voltagePrev)
                        absVotage = voltage - voltagePrev;
                    else
                        absVotage = voltagePrev - voltage;
                    */
                //m_settings.phCalibrationSettings.phFactor=
                //float tmp_wspph=(abs(ph7-ph4))/3.0;
                currentCalibrationPhase = CalibrationPhase::eCalibrationEnd;
            }

            //Serial.println(voltage.as_float(),3);
        }
    }
}

bool PhCalibrationScreen::isConstant(uint16_t loops)
{
    UF16x3 absVotage;

    if (voltage >= voltagePrev)
        absVotage = voltage - voltagePrev;
    else
        absVotage = voltagePrev - voltage;

    if (absVotage < 0.01f)
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
