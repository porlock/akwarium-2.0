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
    uint8_t outPin, period;
    bool go;

public:
    RelayType(uint8_t _outPin = 0)
    {
        outPin = _outPin;
        clock = 0;
        period = 50000;
        go = false;
    };

    void setClockOn()
    {
        if (millis() - clock > 5000)
        {
            Serial.println("raz na 5s");
            clock = millis();
            go = true;
        }

        if (go)
        {
            if (millis() - clock < 500)
            {
                 Serial.println("leje kwas");
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
        Serial.println("kwas stop");
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
    } readings{7, 25, false};

    void readPH()
    {
        readings.ph = 7.30;
    }

    void readTemp()
    {
        readings.temp = 27;
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
    RelayType hciRelay;

    void heaterStart()
    {
    }

    void waterRelayStart()
    {
    }

    void waterRelayStop()
    {
    }

    void heaterStop()
    {
    }

    void hciControll(UF16x2 pH)
    {
        if (pH > 7)
        {
            hciRelay.setClockOn();
        }
        else
        {
            hciRelay.setOff();
        }
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
            hciControll(probing::readings.ph);
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