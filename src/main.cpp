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
#include "PhCalibrationScreen.h"


namespace probing
{
    uint32_t nextUpdateProbing = 0;

    struct ReadingsType
    {
        UF16x2 ph;
        UF16x2 temp;
        bool waterLevel;
    } readings{7, 25, false};

    UF16x2 readPH()
    {
       readings.ph = 7.30;
    }

    UF16x2 readTemp()
    {
        readings.temp = 27;
    }

    bool readWaterLevel()
    {
        readings.waterLevel = true;
    }

    void runProbbingLoop()
    {
        if (millis() - nextUpdateProbing > 100)
        {
            readWaterLevel();
            readPH();
            readTemp();
        }
    }
}

namespace control
{
    void heaterStart(){

    }

    void waterRelayStart()
    {

    }

    void waterRelayStop()
    {
        
    }


    void heaterStop(){

    }

    void hciControll()
    {

    }


    uint32_t nextUpdateControl = 0;

    void refill(bool waterLevel)
    {
        
        if (waterLevel)
        {
            screen::iconsIndicator.refill = waterLevel;
            waterRelayStart();
        }
        else 
        {
            waterRelayStop();
        }
    }

    void runLoop()
    {
        if (millis() - nextUpdateControl > 100)
        {
            nextUpdateControl = millis();
            /*UF16x2 temperature = probing::readTemp();
            UF16x2 ph = probing::readPH();
            */
            refill(probing::readings.waterLevel);
        }
    }
}

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
        probing::runProbbingLoop();
    }

    if (g_currentScreen != g_nextScreen)
    {
        g_currentScreen = g_nextScreen;
    }
}