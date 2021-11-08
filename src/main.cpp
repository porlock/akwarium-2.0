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
#include "MainScreen.h"

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
    Serial.println(g_currentScreen);
    switch (g_currentScreen)
    {
    case eScreenMain:
        //Serial.println("main screen");
        screenMain.control(joystick.readState());
        screenMain.render();
        break;

    case eScreenSettings:
        //Serial.println("setting screen");
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