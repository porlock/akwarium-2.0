#include <MainScreen.h>

   MainScreen::MainScreen(const SettingsType &_settings) : m_settings(_settings)
    {
        m_currentBarPosition = 0;
        m_maxBarPosition = 0;
        m_nextUpdateRead = 0;
        m_nextUpdateJoystick = 0;
        skip = true;
        m_readType = 'P';
        m_screenState = ReadScreens::eScreenReadCorousel;
    }

    void MainScreen::render()
    {
        if (millis() - m_nextUpdateRead > 500)
        {
            m_nextUpdateRead = millis();

            switch (m_screenState)
            {
            case ReadScreens::eScreenReadTemp:
                screen::showReadings(m_currentBarPosition, "Temp", probing::readings.temp);
                break;
            case ReadScreens::eScreenReadPh:
                screen::showReadings(m_currentBarPosition, "Ph", probing::readings.ph);
                break;
            case ReadScreens::eScreenReadCorousel:
                if (skip)
                {
                    m_currentBarPosition--;
                    skip = false;
                }
                else
                {
                    skip = true;
                }
                switch (m_readType)
                {
                case 'P':
                    screen::showReadings(m_currentBarPosition, "Ph", probing::readings.ph, true);
                    break;
                case 'T':
                    screen::showReadings(m_currentBarPosition, "Temp", probing::readings.temp, true);
                    break;
                }
                if (m_currentBarPosition == 0)
                {
                    m_currentBarPosition = m_maxBarPosition;
                    if (m_readType == 'T')
                    {
                        m_readType = 'P';
                    }
                    else
                    {
                        m_readType = 'T';
                    }
                }
                break;
            }
        }
    }

    void MainScreen::control(JoyStatus joyState)
    {
        if (millis() - m_nextUpdateJoystick > 100)
        {
            m_nextUpdateJoystick = millis();
            switch (joyState)
            {
            case JoyStatus::eLeft:
                m_screenState--;
                if (m_screenState < ReadScreens::eScreenReadBegin)
                {
                    m_screenState = ReadScreens::eScreenReadEnd - 1;
                }
                break;
            case JoyStatus::eRight:
                m_screenState++;
                if (m_screenState >= ReadScreens::eScreenReadEnd)
                {
                    m_screenState = ReadScreens::eScreenReadBegin;
                }
                break;
            case JoyStatus::eSelect:
                setNextScreen(ScreenType::eScreenSettings);
                break;
            case JoyStatus::eUp:
            case JoyStatus::eDown:
            case JoyStatus::eCenter:
                break;
            }
        }
    }

    void MainScreen::init()
    {
        m_currentBarPosition = m_settings.screenSettings.barLenght;
        m_maxBarPosition = m_currentBarPosition;
    }


