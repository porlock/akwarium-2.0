#include <Arduino.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <EEPROM.h>

#include "FixedPoint.h"
#include "GlobalEnums.h"
#include "ScreenManager.h"
#include "screen.h"
#include "SettingsType.h"
#include "JoystickType.h"

#include "OptionsScreen.h"

namespace heaterRelay
{
    void start();
    void stop();
}

namespace hciRelay
{
    void start();
    void stop();
}

namespace probing
{

    struct ReadingsType
    {
        UF16x2 ph;
        UF16x2 temp;
        bool waterLevel;
    } readings{7, 25, false};

    UF16x2 readPH()
    {
    }

    UF16x2 readTemp()
    {
    }

    bool readWaterLevel()
    {
        return true;
    }

    void runProbbingLoop()
    {
    }
}

namespace control
{

    uint32_t nextUpdateProbe = 0;

    void ph(UF16x2 ph)
    {
    }

    void refill(bool waterLevel)
    {
        //iconsIndicator->refill = waterLevel;
        screen::iconsIndicator.refill = waterLevel;
        if (waterLevel)
        {
            uint16_t i = 1;
        }
        else
        {
            uint16_t i = 0;
        }
    }

    void runLoop()
    {
        if (millis() - nextUpdateProbe > 500)
        {
            nextUpdateProbe = millis();
            UF16x2 temperature = probing::readTemp();
            UF16x2 ph = probing::readPH();
            refill(probing::readWaterLevel());
        }
    }
}

class MainScreen
{

    enum ReadScreens : uint16_t
    {
        eScreenReadBegin = 1,
        eScreenReadTemp = eScreenReadBegin,
        eScreenReadPh = 2,
        eScreenReadCorousel = 3,
        eScreenReadEnd
    };

    uint16_t m_currentBarPosition = 0;
    uint16_t m_maxBarPosition = 0;
    uint32_t m_nextUpdateRead = 0;
    uint32_t m_nextUpdateJoystick = 0;
    bool skip = true;
    char m_readType = 'P';
    uint16_t m_screenState = ReadScreens::eScreenReadCorousel;

public:
    void render()
    {
        if (millis() - m_nextUpdateRead > 500)
        {
            m_nextUpdateRead = millis();

            switch (m_screenState)
            {
            case ReadScreens::eScreenReadTemp:
                screen::showReadings(m_currentBarPosition, "Temp", 27.5);
                break;
            case ReadScreens::eScreenReadPh:
                screen::showReadings(m_currentBarPosition, "Ph", 7.5);
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
                    screen::showReadings(m_currentBarPosition, "Ph", m_currentBarPosition, true);
                    break;
                case 'T':
                    screen::showReadings(m_currentBarPosition, "Temp", m_currentBarPosition, true);
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

    void control(JoyStatus joyState)
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
            }
        }
    }

    // m_settings jest referencja typu SettingsType
    const SettingsType &m_settings;

    /* 
    m_settings(_settings)
    to jest lista inicjalizacyjna która się odpala przed kodem konstruktora, 
    dzięki temu możesz przypisać referencję w trakcie tworzenia instancji klasy
    */
    MainScreen(const SettingsType &_settings) : m_settings(_settings)
    {
    }

    void init()
    {
        m_currentBarPosition = m_settings.screenSettings.barLenght;
        m_maxBarPosition = m_currentBarPosition;
    }
};

class PhCalibrationScreen
{
    uint16_t i = 0;

public:
    void startPhCalibration()
    {

        if (i < 10)
        {
            screen::showClickOption("Umiesc sade w ph4", 1, 500);
            i++;
        }
        else
        {
            i = 0;
            setNextScreen(ScreenType::eScreenMain);
        }
    }
};

MainScreen screenMain(settings);
OptionsScreen screenOptions(settings);
PhCalibrationScreen screenPhCalibraton;

JoystickType joystick;

void setup()
{
    screen::initializeScreen();
    screen::showLogo();
    Serial.begin(9600);
    Serial.println("Program Start");
    EEPROM.get(0, settings);

    screenMain.init();
    screenOptions.init();
}

void loop()
{
    switch (g_currentScreen)
    {
    case eScreenMain:
        screenMain.control(joystick.readState());
        screenMain.render();
        break;

    case eScreenSettings:
        screenOptions.control(joystick.readState());
        screenOptions.render();
        break;
    case eScreenPhCalibraton:
        screenPhCalibraton.startPhCalibration();
        break;
    }

    if (g_currentScreen != eScreenSettings)
    {
        control::runLoop();
    }

    if (g_currentScreen != g_nextScreen)
    {
        g_currentScreen = g_nextScreen;
    }
}