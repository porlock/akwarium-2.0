#pragma once
#include "GlobalEnums.h"
#include "SettingsType.h"
#include "screen.h"
#include "ScreenManager.h"

class OptionsScreen
{
    bool displaySettingScreens;

    uint32_t m_nextUpdateRead;
    uint32_t m_nextUpdateJoystick;
    uint16_t m_screenState;
    uint16_t m_screenStatusState;
    SettingsType &m_settings;
    enum SettingsScreens : uint16_t;
    enum SettingsStatusScreens : uint16_t;

    struct
    {
        UF16x2 phGrade;
        UF16x2 tempGrade;
        uint16_t timeGrade;
        uint16_t intevalGrade;
    } settingsGrades;

    struct
    {
        UF16x2 phMax;
        UF16x2 phMin;
        UF16x2 tempMin;
        UF16x2 tempMax;
        uint16_t phIntervalMax;
        uint16_t phIntervalMin;
        uint16_t phTimeMax;
        uint16_t phTimeMin;
    } settingsLimits;

    void changeSetting(uint16_t , bool = true);
 
public:
    OptionsScreen(SettingsType);
    void init();
    void render();
    void control(JoyStatus);
};
