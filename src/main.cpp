#include <Arduino.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#define DPIND_SOUNDOUT 6
#define DPIN_TEMPIN 4
#define DPIN_SELECT 12
#define APIN_JOY_X 0
#define APIN_JOY_Y 1
#define APIN_PHIN 6
#define DPIN_KOUT 2
#define DPIN_ZOUT 5
#define HEAT 3

#define SCREEN_READ_TEMP_1 1
#define SCREEN_READ_PH_2 2
#define SCREEN_READ_3 3
#define SCREEN_SETTINGS_4 4
#define SCREEN_PH_CALLIBRATION_5 5
#define SCREEN_OPTIONS_6 6

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

namespace screen
{
    Adafruit_PCD8544 display = Adafruit_PCD8544(11, 10, 9, 8);

       constexpr byte fish[] PROGMEM = {
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

        constexpr byte PROGMEM refresh[]= {
        B00111110,B00000000,B01100011,B00000000,B11000001,B10000000,B10000000,B10000000,
        B10000010,B10100000,B11000001,B11000000,B01100000,B10000000,B00110000,B00000000,
        };

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

    void showReadings(int bottomBarLenght, const char* name, float value, bool showRefreshIcon=0)
    {
        display.clearDisplay();

        display.setCursor(0,0);
        display.setTextColor(BLACK);
        display.setTextSize(1);
        display.print(name); display.println(':');

        display.println();
        display.setTextSize(3);

        if (value >=10) display.println(value,1);
        else display.println(value,2);

        if (showRefreshIcon)
	  	{

	        display.drawBitmap(73,6,refresh,16,8, BLACK);

            display.setTextSize(1);
            for (int i = 0; i < bottomBarLenght; i++)
	        {
                display.print('_');
            }
           
	    }
        display.display();       
    }
    
}

class joystickType
{
    private:
        int horizontal, vertical;
        char status = 'c';
        bool hasChanged;
        void readAnalog()
        {
            horizontal = analogRead(APIN_JOY_X);
	        vertical = analogRead(APIN_JOY_Y);
        }
        void beep(){
            for (unsigned long go = millis(); millis() - go < 100; analogWrite(DPIND_SOUNDOUT, 50));
            digitalWrite(DPIND_SOUNDOUT, LOW);
        }
   
    public:
        char readState()
        {
            readAnalog();
            if (horizontal >700 ) {status = 'l'; beep();}
            else if (horizontal <300) {status = 'r'; beep();}
            else if (vertical > 700) {status = 'd';}
            else if (vertical < 300) {status = 'u';}
            else {status = 'c';}
            return status;
        }
    
};

namespace settings
{
    struct phSettingsType
    {
        float start;
        float stop;
        float interval;
        float onTime;
    } phSettings{6.7, 6.5, 300, 3};

    struct tempSettingsType
    {
        float start;
        float stop;
    };

    struct phCalibrationSettingsType
    {
        float ph7V;
        float ph4V;
        float phFactor;
    };


    struct screenSettingsType
    {
        int barLenght;        
    } screenSettings{13};

};

void setup()
{
    screen::initializeScreen();
    screen::showLogo();
    Serial.begin(9600);
}

void loop()
{ 

    joystickType joystick;
    static int currentBarPozition=settings::screenSettings.barLenght;
    static uint32_t nextUpdateRead, nextUpdateJoystick = 0;
    static char readType = 'P';
    static int screenState = SCREEN_READ_3;
    static bool tmpDelay=false;

    if (millis() > nextUpdateJoystick)
    {
       nextUpdateJoystick = millis() + 300;       
       switch (joystick.readState())
       {
           case 'l':           
            screenState--;
            if (screenState < SCREEN_READ_TEMP_1) { screenState= SCREEN_READ_3;}
            break;
           case 'r':
            screenState++;
            if (screenState > SCREEN_READ_3) { screenState = SCREEN_READ_TEMP_1;}
            break;
       }
    }


    if (millis() > nextUpdateRead)
    {
        
       nextUpdateRead = millis() + 500;

        switch(screenState){
            case SCREEN_READ_TEMP_1:
                screen::showReadings(currentBarPozition,"Temp",26.7);
                break;
            case SCREEN_READ_PH_2:
                screen::showReadings(currentBarPozition,"Ph",7.5);
                break;
            case SCREEN_READ_3:                
                if (tmpDelay)
                {
                    currentBarPozition--; 
                    tmpDelay=false;
                }
                else {tmpDelay=true;}
                
                switch (readType)
                {
                    case 'P':
                        screen::showReadings(currentBarPozition,"Ph",6,true);
                        break;
                    case 'T':
                        screen::showReadings(currentBarPozition,"Temp",27,true);        
                        break;
                }
        
                if  (currentBarPozition==0) {
                    currentBarPozition=settings::screenSettings.barLenght;
                    if (readType == 'T') {readType = 'P';} else {readType = 'T';}
                    }
                break;
        }

    }

}