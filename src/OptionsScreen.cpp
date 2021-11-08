#include <OptionsScreen.h>


    enum SettingsScreens : uint16_t
    {
        eSetBegin = 1,
        eSetTempDown = eSetBegin,
        eSetTempUp = 2,
        eSetPhDown = 3,
        eSetPhUp = 4,
        eSetPhTime = 5,
        eSetPhPeriod = 6,
        eSetSave = 7,
        eSetDefaults = 8,
        eSetPhCalibration = 9,
        eSetEnd
    };

    enum SettingsStatusScreens : uint16_t
    {
        eOptionsSave,
        eOptionsDefault
    };

    uint32_t m_nextUpdateRead = 0;
    uint32_t m_nextUpdateJoystick = 0;
    uint16_t m_screenState = SettingsScreens::eSetBegin;
    uint16_t m_screenStatusState;
    SettingsType &m_settings;

    struct
    {
        UF16x2 phGrade = 0.1f;
        UF16x2 tempGrade = 0.5f;
        uint16_t timeGrade = 1;
        uint16_t intevalGrade = 1;
    } settingsGrades;

    struct
    {
        UF16x2 phMax = 8;
        UF16x2 phMin = 6;
        UF16x2 tempMin = 20;
        UF16x2 tempMax = 35;
        uint16_t phIntervalMax = 60;
        uint16_t phIntervalMin = 1;
        uint16_t phTimeMax = 30;
        uint16_t phTimeMin = 1;
    } settingsLimits;

   // _settings jest referencja typu SettingsType
    OptionsScreen(SettingsType &_settings) : m_settings(_settings)
    {
        displaySettingScreens=true;
    }



    void OptionsScreen::changeSetting(uint16_t screenState, bool add = true)
    {
        switch (screenState)
        {
        case SettingsScreens::eSetTempDown:
            if (add && m_settings.tempSettings.down < settingsLimits.tempMax)
            {
                m_settings.tempSettings.down += settingsGrades.tempGrade;
            }
            else if (!add && m_settings.tempSettings.down > settingsLimits.tempMin)
            {
                m_settings.tempSettings.down -= settingsGrades.tempGrade;
            }
            break;
        case SettingsScreens::eSetTempUp:
            if (add && m_settings.tempSettings.up < settingsLimits.tempMax)
            {
                m_settings.tempSettings.up += settingsGrades.tempGrade;
            }
            else if (!add && m_settings.tempSettings.up > settingsLimits.tempMin)
            {
                m_settings.tempSettings.up -= settingsGrades.tempGrade;
            }
            break;
        case SettingsScreens::eSetPhDown:
            if (add && m_settings.phSettings.down < settingsLimits.phMax)
            {
                m_settings.phSettings.down += settingsGrades.phGrade;
            }
            else if (!add && m_settings.phSettings.down > settingsLimits.phMin)
            {
                m_settings.phSettings.down -= settingsGrades.phGrade;
            }
            break;
        case SettingsScreens::eSetPhUp:
            if (add && m_settings.phSettings.up < settingsLimits.phMax)
            {
                m_settings.phSettings.up += settingsGrades.phGrade;
            }
            else if (!add && m_settings.phSettings.up > settingsLimits.phMin)
            {
                m_settings.phSettings.up -= settingsGrades.phGrade;
            }
            break;
        case SettingsScreens::eSetPhTime:
            if (add && m_settings.phSettings.onTime < settingsLimits.phTimeMax)
            {
                m_settings.phSettings.onTime += settingsGrades.timeGrade;
            }
            else if (!add && m_settings.phSettings.onTime > settingsLimits.phTimeMin)
            {
                m_settings.phSettings.onTime -= settingsGrades.timeGrade;
            }
            break;
        case SettingsScreens::eSetPhPeriod:
            if (add && m_settings.phSettings.interval < settingsLimits.phIntervalMax)
            {
                m_settings.phSettings.interval += settingsGrades.intevalGrade;
            }
            else if (!add && m_settings.phSettings.interval > settingsLimits.phIntervalMin)
            {
                m_settings.phSettings.interval -= settingsGrades.intevalGrade;
            }
            break;
        }
    }

    void OptionsScreen::init()
    {
    }

    void OptionsScreen::render()
    {
        if (displaySettingScreens)
        {
            switch (m_screenState)
            {
            case SettingsScreens::eSetTempUp:
                screen::showSettings(max(settingsLimits.tempMin, m_settings.tempSettings.down), settingsLimits.tempMax, "Gor temp (C):", m_settings.tempSettings.up);
                break;
            case SettingsScreens::eSetTempDown:
                screen::showSettings(settingsLimits.tempMin, settingsLimits.tempMax, "Dol temp (C):", m_settings.tempSettings.down);
                break;
            case SettingsScreens::eSetPhUp:
                screen::showSettings(settingsLimits.phMin, settingsLimits.phMax, "Gor ph:", m_settings.phSettings.up);
                break;
            case SettingsScreens::eSetPhDown:
                screen::showSettings(settingsLimits.phMin, settingsLimits.phMax, "Dol ph:", m_settings.phSettings.down);
                break;
            case SettingsScreens::eSetPhTime:
                screen::showSettings(settingsLimits.phTimeMin, settingsLimits.phTimeMax, "Czas ph (s):", m_settings.phSettings.onTime);
                break;
            case SettingsScreens::eSetPhPeriod:
                screen::showSettings(settingsLimits.phIntervalMin, settingsLimits.phIntervalMax, "Okres ph (m):", m_settings.phSettings.interval);
                break;
            case SettingsScreens::eSetSave:
                screen::showClickOption("Zapis ustawien");
                break;
            case SettingsScreens::eSetDefaults:
                screen::showClickOption("Reset ustawien");
                break;
            case SettingsScreens::eSetPhCalibration:
                screen::showClickOption("Kalibracja PH");
                break;
            }
        }
        else
        {
            switch (m_screenStatusState)
            {
            case SettingsStatusScreens::eOptionsSave:
                screen::showClickOption("Zapisane", 1, 1500);
                break;
            case SettingsStatusScreens::eOptionsDefault:
                screen::showClickOption("Przywrocono", 1, 1500);
            }
            displaySettingScreens = true;
        }
    }

    void OptionsScreen::control(JoyStatus joyState)
    {
        if (millis() - m_nextUpdateJoystick > 100)
        {

            m_nextUpdateJoystick = millis();
            switch (joyState)
            {
            case JoyStatus::eLeft:
                m_screenState--;
                if (m_screenState < SettingsScreens::eSetBegin)
                {
                    m_screenState = SettingsScreens::eSetEnd - 1;
                }
                break;
            case JoyStatus::eRight:
                m_screenState++;
                if (m_screenState >= SettingsScreens::eSetEnd)
                {
                    m_screenState = SettingsScreens::eSetBegin;
                }
                break;
            case JoyStatus::eUp:
                changeSetting(m_screenState);
                break;
            case JoyStatus::eDown:
                changeSetting(m_screenState, false);
                break;
            case JoyStatus::eSelect:
                switch (m_screenState)
                {
                case SettingsScreens::eSetSave:
                    m_screenStatusState = SettingsStatusScreens::eOptionsSave;
                    m_screenState = SettingsScreens::eSetBegin;
                    EEPROM.put(0, settings);
                    displaySettingScreens = false;
                    break;
                case SettingsScreens::eSetDefaults:
                    m_screenStatusState = SettingsStatusScreens::eOptionsDefault;
                    m_screenState = SettingsScreens::eSetBegin;
                    //defreferencja, zapisanie w miesjscu na ktore wskasuje wkaźnik
                    //*settingsPointer = SettingsType();
                    m_settings = defaultSettings;
                    displaySettingScreens = false;
                    break;
                case SettingsScreens::eSetPhCalibration:
                    setNextScreen(ScreenType::eScreenPhCalibraton);
                    break;
                default:
                    setNextScreen(ScreenType::eScreenMain);
                    break;
                }
                break;
            }
        }
    }

