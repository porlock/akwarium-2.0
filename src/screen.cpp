#include <screen.h>

namespace screen
{
    constexpr uint8_t refresh[] PROGMEM= {
        B00111110,B00000000,B01100011,B00000000,B11000001,B10000000,B10000000,B10000000,
        B10000010,B10100000,B11000001,B11000000,B01100000,B10000000,B00110000,B00000000,
        };

    constexpr uint8_t fish[] PROGMEM = {
		B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
		B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
		B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
		B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
		B00000000,B00000000,B00000000,B01111111,B00000000,B00000000,B00000000,
		B00000000,B00000000,B00000011,B10000000,B11100000,B00000000,B00000000,
		B00000000,B00000000,B00001101,B10000000,B00011000,B00000000,B00000000,
		B00000000,B00000000,B00110001,B10000000,B00000110,B00000000,B00000000,
		B00000000,B00000000,B01100101,B00000000,B00000011,B00000000,B00000000,
		B00000111,B00000000,B11001001,B00000000,B00000001,B10000000,B00000000,
		B00001000,B10000001,B00001001,B00000000,B00000000,B11000000,B00000000,
		B00010000,B11000011,B00000001,B00000000,B01111000,B01100000,B00000000,
		B00010000,B01000010,B00000001,B00000000,B11011100,B00100000,B00000000,
		B00010000,B00100100,B00001001,B00000000,B11111100,B00110000,B00000000,
		B00010000,B00100100,B10001001,B00000000,B11111100,B00010000,B00000000,
		B00010000,B00101100,B10000101,B00000000,B11111100,B00011000,B00000000,
		B00010000,B00111000,B10000001,B00000000,B01111000,B00001000,B00000000,
		B00011000,B00011000,B10000001,B00000000,B00000000,B00001000,B00000000,
		B00001000,B00011000,B10000001,B10000000,B00000000,B00001000,B00000000,
		B00000100,B00011000,B01000100,B10000000,B00000000,B00001000,B00000000,
		B00000011,B00011010,B00100100,B10000000,B00000000,B00001000,B00000000,
		B00000011,B00011000,B00010100,B10000000,B00000010,B00001000,B00000000,
		B00000100,B00011000,B00000100,B11000000,B00000000,B11001000,B00000000,
		B00001000,B00011010,B00010100,B01000000,B00000000,B00001000,B00000000,
		B00011000,B00011010,B00010010,B01100000,B00000000,B00001000,B00000000,
		B00010000,B00111001,B00010001,B00100000,B00000000,B00001000,B00000000,
		B00010000,B00101101,B10010000,B10110000,B00000000,B00011000,B00000000,
		B00010000,B00100100,B10010000,B00010000,B00000000,B00010000,B00000000,
		B00010000,B01100100,B00010000,B00011000,B00000000,B00110000,B00000000,
		B00010000,B01000010,B00001000,B01001100,B00000000,B00100000,B00000000,
		B00011000,B11000011,B00001100,B01000110,B00000000,B01100000,B00000000,
		B00001101,B10000001,B10000000,B01000010,B00000000,B11000000,B00000000,
		B00000111,B00000000,B10000000,B00100001,B00000001,B10000000,B00000000,
		B00000000,B00000000,B01100000,B00010000,B11000011,B00000000,B00000000,
		B00000000,B00000000,B00110000,B00001100,B01100110,B00000000,B00000000,
		B00000000,B00000000,B00001100,B00000000,B00111000,B00000000,B00000000,
		B00000000,B00000000,B00000011,B11000000,B11100000,B00000000,B00000000,
		B00000000,B00000000,B00000000,B01111111,B00000000,B00000000,B00000000,
		B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
		B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
		};   


    IconsIndicatorsType iconsIndicator{0, 0, 0, 0};

    Adafruit_PCD8544 display = Adafruit_PCD8544(11, 10, 9, 8);

    void initializeScreen()
    {
        display.begin();
        display.setContrast(60);
        display.clearDisplay();
    }

    void clearScreen()
    {
        display.clearDisplay();
        display.display();
    }

    void showLogo()
    {
        display.drawBitmap(14, 4, fish, 56, 40, BLACK);
        display.display();
        delay(5000);
        clearScreen();
    }

    void showReadings(uint16_t bottomBarLenght, const char *name, UF16x2 value, bool showRefreshIcon)
    {
        display.clearDisplay();
        display.setTextColor(BLACK);
        display.setTextSize(1);

        if (iconsIndicator.error)
        {
            display.setCursor(30, 0);
            display.print("!");
        };

        if (iconsIndicator.refill)
        {
            display.setCursor(40, 0);
            display.print("D");
        };
        if (iconsIndicator.heater)
        {
            display.setCursor(50, 0);
            display.print("G");
        };
        if (iconsIndicator.hcl)
        {
            display.setCursor(60, 0);
            display.print("K");
        };

        display.setCursor(0, 0);
        display.print(name);
        display.println(':');

        display.println();
        display.setTextSize(3);
        if (value >= 10)
            display.println(value.as_float(), 1);
        else
            display.println(value.as_float(), 2);

        if (showRefreshIcon)
        {

            display.drawBitmap(73, 6, refresh, 16, 8, BLACK);

            display.setTextSize(1);
            for (uint16_t i = 0; i < bottomBarLenght; i++)
            {
                display.print('_');
            }
        }
        display.display();
    }

    void showSettings(UF16x2 minValue, UF16x2 maxValue, const char *name, UF16x2 value)
    {
        display.clearDisplay();
        display.setTextSize(1);
        display.println("USTAWIENIA");
        display.println(name);
        display.print(minValue.as_float());
        display.print(" - ");
        display.println(maxValue.as_float());
        display.println();
        display.setTextSize(2);
        display.println(value.as_float());
        display.display();
    }

    void showCalibration(CalibrationPhase phase, UF32x3 voltage, bool blink){
	    display.clearDisplay();
		display.setTextSize(1);
		display.print("(");
        display.print(phase);
        display.print("/");
        display.print(CalibrationPhase::eCalibrationEnd-1);
        display.println(")");
        switch (phase)
        {
            case CalibrationPhase::eCalibrationPh4Stable:
                display.println("Stabilizacja PH=4");
                display.print("V: ");
		        display.print(voltage.as_float(),3);
                break;
            case CalibrationPhase::eCalibrationPh7Stable:
                display.println("Stabilizacja PH=7");
                display.print("V: ");
		        display.print(voltage.as_float(),3);
                break;
            case CalibrationPhase::eCalibrationPh7Read:
                display.println("Umiesc sonde");
				display.println("w PH=7");
                break;
            case CalibrationPhase::eCalibrationPh4Read:
                display.println("Umiesc sonde");
				display.println("w PH=4");
                break;
            case CalibrationPhase::eCalibrationEnd:
                break;
        }
		//display.println(" " + String(ile/5)+"%");
		if (blink) display.println("PROSZE CZEKAC");
		display.display();
    }

    void showSettings(uint16_t minValue, uint16_t maxValue, const char *name, uint16_t value)
    {
        display.clearDisplay();
        display.setTextSize(1);
        display.println("USTAWIENIA");
        display.println(name);
        display.print(minValue);
        display.print(" - ");
        display.println(maxValue);
        display.println();
        display.setTextSize(2);
        display.println(value);
        display.display();
    }


    void showClickOption(const char *text, uint16_t fontSize, uint16_t pause)
    {
        display.clearDisplay();
        display.setTextSize(fontSize);
        display.println(text);
        display.display();
        delay(pause);
    }

    void showSave()
    {
        display.clearDisplay();
        display.setTextSize(1);
        display.println("Ustawienia");
        display.println("zapisane");
        display.display();
    }

}