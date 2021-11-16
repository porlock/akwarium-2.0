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
            m_voltage = probing::readings.phVoltage;
            m_blink = false;
            
            if (m_currentCalibrationPhase == CalibrationPhase::eCalibrationPh7Read && (m_voltage < 3.1) && (m_voltage > 2.9))
            {
                m_currentCalibrationPhase = CalibrationPhase::eCalibrationPh7Stable;
                m_blink = !m_blink;
                m_constLoopsCounter = 0;
            }
            else if (m_currentCalibrationPhase == CalibrationPhase::eCalibrationPh7Stable && isConstant())
            {
                m_currentCalibrationPhase = CalibrationPhase::eCalibrationPh4Read;
                m_settings.phCalibrationSettings.ph7V = m_voltage;
                m_blink = false;
            }
            else if (m_currentCalibrationPhase == CalibrationPhase::eCalibrationPh4Read && (m_voltage < 3.50) and (m_voltage > 3.30))
            {
                m_currentCalibrationPhase = CalibrationPhase::eCalibrationPh7Stable;
                m_constLoopsCounter = 0;
                m_blink = !m_blink;
            }
            else if (m_currentCalibrationPhase == CalibrationPhase::eCalibrationPh4Stable && isConstant())
            {
                UF16x2 absVotage;
                if (m_settings.phCalibrationSettings.ph7V >= m_voltage)
                    absVotage = m_settings.phCalibrationSettings.ph7V - m_voltage;
                else
                    absVotage = m_voltage - m_settings.phCalibrationSettings.ph7V;

                m_settings.phCalibrationSettings.phFactor = absVotage / 3.0f;
                m_blink = false;
                setNextScreen(ScreenType::eScreenSettings);
            }
        }
    }
}

bool PhCalibrationScreen::isConstant(uint16_t loops)
{
    UF16x2 absVotage;

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
