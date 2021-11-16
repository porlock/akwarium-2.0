#pragma once
#include "SettingsType.h"
#include "GlobalEnums.h"
#include "screen.h"
#include "ScreenManager.h"
#include "probing.h"

class MainScreen
{
// public type declarations
public:
    enum ReadScreens : uint16_t
    {
        eScreenReadBegin = 1,
        eScreenReadTemp = eScreenReadBegin,
        eScreenReadPh = 2,
        eScreenReadCorousel = 3,
        eScreenReadEnd
    };

// private member declarations
private:
    uint16_t m_currentBarPosition;
    uint16_t m_maxBarPosition;
    uint32_t m_nextUpdateRead;
    uint32_t m_nextUpdateJoystick;
    bool skip;
    char m_readType;
    const SettingsType &m_settings;
    uint16_t m_screenState;

// public method declarations
public:
    void render();
    void control(JoyStatus );
    MainScreen(const SettingsType&);
    void init();
   
};

