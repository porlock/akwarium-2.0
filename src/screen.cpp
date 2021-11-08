#include <stdint.h>
#include <screen.h>

namespace screen
{

    IconsIndicatorsType iconsIndicator{0, 0, 0};

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
        if (iconsIndicator.hci)
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
            display.println(value.asFloat(), 1);
        else
            display.println(value.asFloat(), 2);

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
        display.print(minValue.asFloat());
        display.print(" - ");
        display.println(maxValue.asFloat());
        display.println();
        display.setTextSize(2);
        display.println(value.asFloat());
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