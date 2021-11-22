#pragma once
#include "SettingsType.h"
#include "GlobalEnums.h"
#include "screen.h"
#include "ScreenManager.h"
#include <EEPROM.h>
#include "probing.h"

class OptionsScreen
{
// public type declarations
public:

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

    struct SettingsGradesType
    {
        F32x3 phGrade = 0.1f;
        F32x3 tempGrade = 0.5f;
        uint16_t timeGrade = 1;
        uint16_t intevalGrade = 1;
    };

    struct SettingsLimitsType
    {
        F32x3 phMax = 8;
        F32x3 phMin = 6;
        F32x3 tempMin = 20;
        F32x3 tempMax = 35;
        uint16_t phIntervalMax = 60;
        uint16_t phIntervalMin = 1;
        uint16_t phTimeMax = 30;
        uint16_t phTimeMin = 1;
    };

// private method declarations
private:

    void changeSetting(uint16_t , bool  = true);
 
// public method declarations
public:

    OptionsScreen(SettingsType&);
    void init();
    void render();
    void control(JoyStatus);

// private member declarations
private:


    SettingsType &m_settings;
    uint32_t m_nextUpdateRead;
    uint32_t m_nextUpdateJoystick;
    uint16_t m_screenState;
    uint16_t m_screenStatusState;
    bool m_displaySettingScreens;
    
    SettingsGradesType settingsGrades;
    SettingsLimitsType settingsLimits;
};
