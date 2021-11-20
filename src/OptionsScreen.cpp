#include "OptionsScreen.h"

OptionsScreen::OptionsScreen(SettingsType &_settings)
    : m_settings(_settings),
      m_nextUpdateRead(0),
      m_nextUpdateJoystick(0),
      m_screenState(SettingsScreens::eSetBegin),
      m_displaySettingScreens(true) {}

void OptionsScreen::changeSetting(uint16_t screenState, bool add)
{
    switch (screenState)
    {
    case SettingsScreens::eSetTempDown:
        if (add && m_settings.tempSettings.down < min(settingsLimits.tempMax, m_settings.tempSettings.up - settingsGrades.tempGrade))
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
        else if (!add && m_settings.tempSettings.up > max(settingsLimits.tempMin, m_settings.tempSettings.down + settingsGrades.tempGrade))
        {
            m_settings.tempSettings.up -= settingsGrades.tempGrade;
        }
        break;
    case SettingsScreens::eSetPhDown:
        if (add && m_settings.phSettings.down <  min(settingsLimits.phMax, m_settings.phSettings.up - settingsGrades.phGrade))
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
        else if (!add && m_settings.phSettings.up > max(settingsLimits.phMin, m_settings.phSettings.down + settingsGrades.phGrade))
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
    if (millis() - m_nextUpdateRead > 500)
    {
        m_nextUpdateRead = millis();
        if (m_displaySettingScreens)
        {
            switch (m_screenState)
            {
            case SettingsScreens::eSetTempDown:
                screen::showSettings(settingsLimits.tempMin, min(settingsLimits.tempMax, m_settings.tempSettings.up - settingsGrades.tempGrade), "Dol temp (C):", m_settings.tempSettings.down);
                break;
            case SettingsScreens::eSetTempUp:
                screen::showSettings(max(settingsLimits.tempMin, m_settings.tempSettings.down + settingsGrades.tempGrade), settingsLimits.tempMax, "Gor temp (C):", m_settings.tempSettings.up);
                break;
            case SettingsScreens::eSetPhDown:
                screen::showSettings(settingsLimits.phMin, min(settingsLimits.phMax, m_settings.phSettings.up - settingsGrades.phGrade), "Dol ph:", m_settings.phSettings.down);
                break;
            case SettingsScreens::eSetPhUp:
                screen::showSettings(max(settingsLimits.phMin, m_settings.phSettings.down + settingsGrades.phGrade), settingsLimits.phMax, "Gor ph:", m_settings.phSettings.up);
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
                screen::showCalibrationInfo(m_settings.phCalibrationSettings.ph7V, m_settings.phCalibrationSettings.ph4V, m_settings.phCalibrationSettings.phFactor, probing::readings.phVoltage);
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
            m_displaySettingScreens = true;
        }
    }
}

void OptionsScreen::control(JoyStatus joyState)
{
    if (millis() - m_nextUpdateJoystick > 100)
    {

        m_nextUpdateJoystick = millis();
        switch (joyState)
        {
        case JoyStatus::eCenter:
            break;
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
                m_displaySettingScreens = false;
                break;
            case SettingsScreens::eSetDefaults:
                m_screenStatusState = SettingsStatusScreens::eOptionsDefault;
                m_screenState = SettingsScreens::eSetBegin;
                // defreferencja, zapisanie w miesjscu na ktore wskasuje wkaźnik
                //*settingsPointer = SettingsType();
                m_settings = defaultSettings;
                m_displaySettingScreens = false;
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
