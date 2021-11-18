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
#include "RelayType.h"
#include "probing.h"
#include "SMA.h"

namespace control
{
    RelayType hclRelay(Pins::eDigitalPinHclOut);
    RelayType heaterRelay(Pins::eDigitalPinHeaterOut, true);
    RelayType waterRelay(Pins::eDigitalPinWaterOut);
    bool hclOn = false;
    bool tempOn = false;

    void heaterControl(F16x3 temp)
    {
        if (temp < settings.tempSettings.down || tempOn)
        {
            screen::iconsIndicator.heater = true;
            heaterRelay.setOn();
            tempOn = true;
        }
        if (temp > settings.tempSettings.down)
        {
            screen::iconsIndicator.heater = false;
            heaterRelay.setOff();
            tempOn = false;
        }
    }

    void hclControll(F16x3 pH)
    {
        if (pH > settings.phSettings.up || hclOn)
        {
            screen::iconsIndicator.hcl = true;
            hclRelay.setClockOn(settings.phSettings.onTime * 1000, settings.phSettings.interval * 60000);
            hclOn = true;
        }
        if (pH < settings.phSettings.down)
        {
            screen::iconsIndicator.hcl = false;
            hclOn = false;
            hclRelay.setOff();
        }
    }

    void waterLevelControl(bool waterLevel)
    {
        if (waterLevel)
        {
            screen::iconsIndicator.refill = true;
            waterRelay.setOn();
        }
        else
        {
            waterRelay.setOff();
            screen::iconsIndicator.refill = false;
        }
    }

    uint32_t nextUpdateControl = 0;
    void runLoop()
    {
        if (millis() - nextUpdateControl > 500)
        {
            nextUpdateControl = millis();
            hclControll(probing::readings.ph);
            heaterControl(probing::readings.temp);
            waterLevelControl(probing::readings.waterLevel);
        }
    }
}

MainScreen screenMain(settings);
OptionsScreen screenOptions(settings);
PhCalibrationScreen screenPhCalibraton(settings);
JoystickType joystick;

void setup()
{
    control::hclRelay.initPin();
    control::heaterRelay.initPin();
    control::waterRelay.initPin();
    joystick.initPin();
    screen::initializeScreen();
    screen::showLogo();
    Serial.begin(9600);
    Serial.println("Program Start");
    EEPROM.get(0, settings);
    if (settings.eepromCheck != defaultSettings.eepromCheck)
    {
        screen::showMemmoryError();
        settings = defaultSettings;
    }
    screenMain.init();
    screenOptions.init();
    probing::initTemperatureSensor();
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
        screenPhCalibraton.control(joystick.readState());
        screenPhCalibraton.render();
        break;
    }

    if (g_currentScreen != eScreenSettings)
    {
        probing::runLoop();
        control::runLoop();
        //Serial.println(analogRead(Pins::eAnalogPinPH));
        //delay(500);
    }

    if (g_currentScreen != g_nextScreen)
    {
        g_currentScreen = g_nextScreen;
    }
}