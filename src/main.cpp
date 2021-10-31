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

#define SCREEN_SET_UP_TEMP 1
#define SCREEN_SET_DOWN_TEMP 2
#define SCREEN_SET_UP_PH 3
#define SCREEN_SET_DOWN_PH 4
#define SCREEN_SET_TIME_PH 5
#define SCREEN_SET_PERIOD_PH 6

#define SCREEN_PH_CALLIBRATION_5 10
#define SCREEN_OPTIONS_6 11


enum ScreenType
{
    ST_Main,
    ST_Options
};
 
ScreenType g_currentScreen = ST_Main;
ScreenType g_nextScreen = ST_Main;

void setNextScreen(ScreenType screen)
{
    g_nextScreen = screen;
}
 
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


    void showSettings(int max, int min, const char* name, float value)
    {
        display.clearDisplay();
        display.setTextSize(1);
	    display.println("USTAWIENIA");
  	    display.println(name);
		display.print("od "); display.print(min); display.print(" do "); display.println(max);
        display.println();
		display.setTextSize(2);
		display.println(value);   
        display.display(); 
    }
}
class joystickType
{
    private:
        int horizontal, vertical, select;
        char status = 'c';
        char prevStatus = 'c';
        void readPins()
        {
            horizontal = analogRead(APIN_JOY_X);
	        vertical = analogRead(APIN_JOY_Y);
            select = digitalRead(DPIN_SELECT);            
        }
        void beep(){
            for (unsigned long go = millis(); millis() - go < 100; analogWrite(DPIND_SOUNDOUT, 50));
            digitalWrite(DPIND_SOUNDOUT, LOW);
        }
   
    public:
        char readState()
        {
            readPins();
            if (!select) {status = 'p'; }
            else if (horizontal >700 ) {status = 'l'; }
            else if (horizontal <300) {status = 'r'; }
            else if (vertical > 700) {status = 'd';}
            else if (vertical < 300) {status = 'u';}
            else {status = 'c';}
          
            if (status == prevStatus){
                return 'c';                
            }
            prevStatus=status;            
            beep();
            return status;
        }
    
};

namespace settings
{
    struct phSettingsType
    {
        float up;
        float down;
        float interval;
        float onTime;
    } phSettings{6.7, 6.5, 300, 3};

    struct temptSettingsType
    {
        float up;
        float down;
    }tempSettings{27,25.5};

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


class MainScreen
{
    int m_currentBarPozition = settings::screenSettings.barLenght;
    uint32_t m_nextUpdateRead = 0;
    uint32_t m_nextUpdateJoystick = 0;
    char m_readType = 'P';
    int m_screenState = SCREEN_READ_3;
 
    public:
 
    void render()
    {
        if (millis() > m_nextUpdateRead)
        {   
            m_nextUpdateRead = millis() + 1000;
            switch(m_screenState)
            {
                case SCREEN_READ_TEMP_1:
                    screen::showReadings(m_currentBarPozition,"Temp",26.7);
                    break;
                case SCREEN_READ_PH_2:
                    screen::showReadings(m_currentBarPozition,"Ph",7.5);
                    break;
                case SCREEN_READ_3:
                    m_currentBarPozition--; 
                    switch (m_readType)
                    {
                        case 'P':
                            screen::showReadings(m_currentBarPozition,"Ph",m_currentBarPozition,true);
                            break;
                        case 'T':
                            screen::showReadings(m_currentBarPozition,"Temp",m_currentBarPozition,true);        
                            break;
                    }
            
                    if  (m_currentBarPozition==0) {
                        m_currentBarPozition=settings::screenSettings.barLenght;
                        if (m_readType == 'T') {m_readType = 'P';} else {m_readType = 'T';}
                        }
                    break;
            }
 
        }  
    }
 
    void control(char joyState)
    {
        if (millis() > m_nextUpdateJoystick)
        {
            m_nextUpdateJoystick = millis() + 100;       
            switch (joyState)
            {
                case 'l':           
                m_screenState--;
                if (m_screenState < SCREEN_READ_TEMP_1) { m_screenState= SCREEN_READ_3;}
                break;
                case 'r':
                m_screenState++;
                if (m_screenState > SCREEN_READ_3) { m_screenState = SCREEN_READ_TEMP_1;}
                break;
                case 'p':
                setNextScreen(ScreenType::ST_Options);
                break;
            }
        }
    }
};
 
 
class OptionsScreen
{
    int m_currentBarPozition = settings::screenSettings.barLenght;
    uint32_t m_nextUpdateRead = 0;
    uint32_t m_nextUpdateJoystick = 0;
    int m_screenState = SCREEN_SET_UP_TEMP;
    settings::temptSettingsType temptSettingsType;

    void changeSetting (int screenState,bool add=true, double grade=0.1)
    {   
        switch(screenState)
        {
            case SCREEN_SET_UP_TEMP:
                if (add){temptSettingsType.up=temptSettingsType.up+grade;} else {temptSettingsType.up=temptSettingsType.up-grade;}
            break;
            case SCREEN_SET_DOWN_TEMP:
            
            break;
            case SCREEN_SET_UP_PH:
            
            break;
            case SCREEN_SET_DOWN_PH:
            
            break;
            case SCREEN_SET_TIME_PH:
            
            break;
            case SCREEN_SET_PERIOD_PH:
            
            break;
        }
    }




    public:
 
    void render()
    {    
            switch(m_screenState)
            {
                case SCREEN_SET_UP_TEMP:
                screen::showSettings(35,20,"Gor temp (C):",temptSettingsType.up);
                break;
                case SCREEN_SET_DOWN_TEMP:
                screen::showSettings(35,20,"Dol temp (C):",1);
                break;
                case SCREEN_SET_UP_PH:
                screen::showSettings(0,14,"Gor ph:",1);
                break;
                case SCREEN_SET_DOWN_PH:
                screen::showSettings(0,14,"Dol ph:",1);
                break;
                case SCREEN_SET_TIME_PH:
                screen::showSettings(0,60,"Czas ph (s):",1);
                break;
                case SCREEN_SET_PERIOD_PH:
                screen::showSettings(0,20,"Czas ph (m):",1);
                break;
            }
         
         

        
    }

 
    void control(char joyState)
    {
        if (millis() > m_nextUpdateJoystick)
        {
            
            m_nextUpdateJoystick = millis() + 100;       
            switch (joyState)
            {
                case 'l':           
                m_screenState--;
                if (m_screenState < SCREEN_SET_UP_TEMP) { m_screenState= SCREEN_SET_PERIOD_PH;}
                break;
                case 'r':
                m_screenState++;
                if (m_screenState > SCREEN_SET_PERIOD_PH) { m_screenState = SCREEN_SET_UP_TEMP;}
                break;
                case 'u':
                    changeSetting(m_screenState);
                break;
                case 'd':
                    changeSetting(m_screenState,false);
                break;
                case 'p':
                setNextScreen(ScreenType::ST_Main);
                break;
            }
        }
    }
};
 
MainScreen screenMain;
OptionsScreen screenOptions;
joystickType joystick;

void setup()
{
    screen::initializeScreen();
    screen::showLogo();
    Serial.begin(9600);
}

void loop()
{ 
    
    switch(g_currentScreen)
    {
        case ST_Main:
            screenMain.control(joystick.readState());
            screenMain.render();
            break;
 
        case ST_Options:
            screenOptions.control(joystick.readState());
            screenOptions.render();
            break;
    }
 
    if (g_currentScreen != g_nextScreen)
    {
        g_currentScreen = g_nextScreen;
    }

}