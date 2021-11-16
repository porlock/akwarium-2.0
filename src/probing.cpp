#include <probing.h>

namespace probing
{

    uint32_t nextUpdateProbing = 0;
    ReadingsType readings{6.5, 27, 0, false};

    bool add = true;

    SMA<20> voltageAnalogFilter;

    /*
    UF16x2 readV(uint16_t pause, uint16_t loops)
    {
        SMA<30> filter;
        uint32_t analogRead;
        for (uint8_t i = 0; i < loops; i++)
        {
            analogRead = filter(Pins::eAnalogPinPH);
            delay(pause);
        }
        return (5.0f / 1024.0f) * analogRead;
    }*/

    void readPH()
    {

        uint32_t analogRead = voltageAnalogFilter(Pins::eAnalogPinPH);
        readings.phVoltage = (5.0f / 1024.0f) * analogRead;
        readings.ph = (UF16x2)7.0f + ((settings.phCalibrationSettings.ph7V - readings.phVoltage) / settings.phCalibrationSettings.phFactor);

        /*
        if (readings.ph < 7.5 && add)
        {
            readings.ph += 0.01f;
        }
        else
            add = false;

        if (readings.ph > 6.0f && !add)
        {
            readings.ph -= 0.01f;
        }
        else
            add = true;
            */
    }

    void readTemp()
    {
        readings.temp = 24;
    }

    void readWaterLevel()
    {
        readings.waterLevel = false;
    }

    void runLoop()
    {
        if (millis() - nextUpdateProbing > 500)
        {
            nextUpdateProbing = millis();
            readWaterLevel();
            readPH();
            readTemp();
        }
    }
}