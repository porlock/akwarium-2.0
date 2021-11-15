#include <probing.h>

namespace probing
{
    uint32_t nextUpdateProbing = 0;

    ReadingsType readings{6.5, 27, false};

    bool add = true;


    uint16_t readV( uint16_t delay, uint16_t loops)
    {
        return 3;
    }

    void readPH()
    {

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