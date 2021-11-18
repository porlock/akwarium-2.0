#include <PhCalibrationScreen.h>

PhCalibrationScreen::PhCalibrationScreen(SettingsType &_settings)
    : m_settings(_settings),
      m_currentCalibrationPhase(CalibrationPhase::eCalibrationStart),
      m_constLoopsCounter(0),
      m_voltage(0),
      m_voltagePrev(0),
      m_blink(false) {}

void PhCalibrationScreen::control(JoyStatus joyState)
{
    if (millis() - m_nextUpdateJoystick > 100)
    {
        m_nextUpdateJoystick = millis();
        if (joyState == JoyStatus::eSelect)
            setNextScreen(ScreenType::eScreenSettings);
        else
        {
            probing::readPH();

            Serial.println(m_blink);
            m_voltage = probing::readings.phVoltage;

            if (m_currentCalibrationPhase == CalibrationPhase::eCalibrationPh7Stable or m_currentCalibrationPhase == CalibrationPhase::eCalibrationPh4Stable)
                m_blink = !m_blink;
            else
                m_blink = false;

            if (m_currentCalibrationPhase == CalibrationPhase::eCalibrationPh7Read && (m_voltage < 3.1) && (m_voltage > 2.9))
            {
                if (m_currentCalibrationPhase < CalibrationPhase::eCalibrationPh7Stable)
                {
                    m_currentCalibrationPhase = CalibrationPhase::eCalibrationPh7Stable;
                    m_constLoopsCounter = 0;
                }
            }
            else if (m_currentCalibrationPhase == CalibrationPhase::eCalibrationPh7Stable && isConstant())
            {
                if (m_currentCalibrationPhase < CalibrationPhase::eCalibrationPh4Read)
                {
                    m_currentCalibrationPhase = CalibrationPhase::eCalibrationPh4Read;
                    m_settings.phCalibrationSettings.ph7V = m_voltage;
                }
            }
            else if (m_currentCalibrationPhase == CalibrationPhase::eCalibrationPh4Read && (m_voltage < 3.50) and (m_voltage > 3.30))
            {
                if (m_currentCalibrationPhase < CalibrationPhase::eCalibrationPh4Stable)
                {
                    m_currentCalibrationPhase = CalibrationPhase::eCalibrationPh4Stable;
                    m_constLoopsCounter = 0;
                }
            }
            else if (m_currentCalibrationPhase == CalibrationPhase::eCalibrationPh4Stable && isConstant())
            {                
                F16x3 absVotage;
                if (m_settings.phCalibrationSettings.ph7V >= m_voltage)
                    absVotage = m_settings.phCalibrationSettings.ph7V - m_voltage;
                else
                    absVotage = m_voltage - m_settings.phCalibrationSettings.ph7V;

                m_settings.phCalibrationSettings.phFactor = absVotage.as_float() / 3.0f;
                setNextScreen(ScreenType::eScreenSettings);
            }
        }
    }
}

bool PhCalibrationScreen::isConstant(uint16_t loops)
{
    F16x3 absVotage;

    if (m_voltage >= m_voltagePrev)
        absVotage = m_voltage - m_voltagePrev;
    else
        absVotage = m_voltagePrev - m_voltage;

    if (absVotage <= 0.01f)
    {
        if (m_constLoopsCounter >= loops)
        {
            return true;
        }
        m_constLoopsCounter++;
    }
    else
    {
        m_voltagePrev = m_voltage;
        m_constLoopsCounter = 0;
    }
    return false;
}

void PhCalibrationScreen::render()
{
    if (millis() - m_nextUpdateRead > 500)
    {
        m_nextUpdateRead = millis();
        screen::showCalibration(m_currentCalibrationPhase, m_voltage, m_blink);
    }
}
