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

class RelayType
{

private:
    uint32_t clock;
    uint8_t outPin;
    uint8_t period;
    bool inverted;
    bool go;

public:
    RelayType(uint8_t _outPin, bool invertedLogic = false)
    {
        outPin = _outPin;
        clock = 0;
        go = false;
        inverted = invertedLogic;
    };

    void initPin()
    {
        pinMode(outPin, OUTPUT);
    }

    void setOn()
    {
        if (inverted)
            digitalWrite(outPin, LOW);
        else
            digitalWrite(outPin, HIGH);
    };

    void setClockOn(uint32_t onTimeMilis, uint32_t intervalMilis)
    {
        if (millis() - clock > intervalMilis)
        {
            clock = millis();
            go = true;
        }

        if (go)
        {
            if (millis() - clock < onTimeMilis)
            {
                void setOn();
            }
            else
            {
                setOff();
                go = false;
            }
        }
    };

    void setOff()
    {
        if (inverted)
            digitalWrite(outPin, HIGH);
        else
            digitalWrite(outPin, LOW);
    }
};

namespace probing
{
    uint32_t nextUpdateProbing = 0;

    struct ReadingsType
    {
        UF16x2 ph;
        UF16x2 temp;
        bool waterLevel;
    } readings{6.5, 27, false};

    void readPH()
    {
        readings.ph = 7.30;
    }

    void readTemp()
    {
        readings.temp = 24;
    }

    bool readWaterLevel()
    {
        readings.waterLevel = true;
    }

    void runLoop()
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
    RelayType hclRelay(Pins::eDigitalPinHclOut);
    RelayType heaterRelay(Pins::eDigitalPinHeaterOut, true);
    RelayType waterRelay(Pins::eDigitalPinWaterOut);
    bool hclOn = false;
    bool tempOn = false;

    void heaterControl(UF16x2 temp)
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

    void hclControll(UF16x2 pH)
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
        if (millis() - nextUpdateControl > 100)
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
PhCalibrationScreen screenPhCalibraton;
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
        probing::runLoop();
        control::runLoop();
    }

    if (g_currentScreen != g_nextScreen)
    {
        g_currentScreen = g_nextScreen;
    }
}