#pragma once
#include "SettingsType.h"
#include "GlobalEnums.h"
#include "screen.h"
#include "ScreenManager.h"

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
        UF16x2 phGrade;
        UF16x2 tempGrade;
        uint16_t timeGrade;
        uint16_t intevalGrade;
    };

    struct SettingsLimitsType
    {
        UF16x2 phMax;
        UF16x2 phMin;
        UF16x2 tempMin;
        UF16x2 tempMax;
        uint16_t phIntervalMax;
        uint16_t phIntervalMin;
        uint16_t phTimeMax;
        uint16_t phTimeMin;
    };

// private method declarations
private:

    void changeSetting(uint16_t screenState, bool add = true);
 
// public method declarations
public:

    OptionsScreen(SettingsType& _settings);
    void init();
    void render();
    void control(JoyStatus status);

// private member declarations
private:

    bool displaySettingScreens;

    uint32_t m_nextUpdateRead;
    uint32_t m_nextUpdateJoystick;
    uint16_t m_screenState;
    uint16_t m_screenStatusState;
    SettingsType &m_settings;
    SettingsGradesType settingsGrades;
    SettingsLimitsType settingsLimits;
};
