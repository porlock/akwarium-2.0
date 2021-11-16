#include <probing.h>

namespace probing
{

    uint32_t nextUpdateProbing = 0;
    ReadingsType readings{6.5, 27, 0, false};

    bool aquariumTempError = false;
    SMA<20> voltageAnalogFilter;

    // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
    OneWire oneWire(Pins::eDigitalPinTempIn);
    // Pass our oneWire reference to Dallas Temperature.
    DallasTemperature sensors(&oneWire);
    // arrays to hold device addresses
    DeviceAddress aquariumThermometer;

    void initTemperatureSensor()
    {
        sensors.begin();
        // search for devices on the bus and assign based on an index.
        if (!sensors.getAddress(aquariumThermometer, 0))
        {
            Serial.println("Unable to find address for Device 0");
            screen::iconsIndicator.error = true;
            aquariumTempError = true;
        }
    }

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
    }

    void readTemp()
    {
        if (aquariumTempError)
            readings.temp = 0;
        else
            readings.temp = sensors.getTempC(aquariumThermometer);
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