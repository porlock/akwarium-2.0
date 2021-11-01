#include <Arduino.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <EEPROM.h>


enum Pins : uint16_t{
    digitalPinSountOut=6,
    digitalPinTempIn=4,
    digitalPinJoySelect=12,
    analogPinJoyX=0,
    analogPinJoyY=1
};


/*
#define DPIND_SOUNDOUT 6
#define DPIN_TEMPIN 4
#define DPIN_SELECT 12
#define APIN_JOY_X 0
#define APIN_JOY_Y 1
#define APIN_PHIN 6
#define DPIN_KOUT 2
#define DPIN_ZOUT 5
#define HEAT 3
*/


enum ScreenType
{
    mainScreen,
    settingsScreen,
    saveScreen,
};

ScreenType g_currentScreen = mainScreen;
ScreenType g_nextScreen = mainScreen;

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


    struct IconsIndicatorsType{
        bool refill;
        bool hci;
        bool heater;
    } iconsIndicator {0,0,0};

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

    void showReadings(uint16_t bottomBarLenght, const char* name, float value, bool showRefreshIcon=false)
    {
        display.clearDisplay();
        display.setTextColor(BLACK);
        display.setTextSize(1);
        
        if (iconsIndicator.refill){display.setCursor(40,0); display.print("D");};
        if (iconsIndicator.heater){display.setCursor(50,0); display.print("G");};
        if (iconsIndicator.hci){display.setCursor(60,0); display.print("K");};

        display.setCursor(0,0);
        display.print(name); display.println(':');
    

        display.println();
        display.setTextSize(3);

        if (value >=10) display.println(value,1);
        else display.println(value,2);

        if (showRefreshIcon)
	  	{

	        display.drawBitmap(73,6,refresh,16,8, BLACK);

            display.setTextSize(1);
            for (uint16_t i = 0; i < bottomBarLenght; i++)
	        {
                display.print('_');
            }
           
	    }
        display.display();       
    }


    void showSettings(float minValue, float maxValue, const char* name, float value)
    {
        display.clearDisplay();
        display.setTextSize(1);
	    display.println("USTAWIENIA");
  	    display.println(name);
		display.print("od "); display.print(maxValue); display.print(" do "); display.println(minValue);
        display.println();
		display.setTextSize(2);
		display.println(value);   
        display.display(); 
    }

    void showSave()
    {
        display.clearDisplay();
        display.setTextSize(1);
		display.println("Ustawienia");
        display.println("zapisane");   
        display.display();     
        delay(1500);  
    }

}
class JoystickType
{
    private:
        uint16_t horizontal, vertical, select;
        char status = 'c';
        char prevStatus = 'c';
        void readPins()
        {
            horizontal = analogRead(Pins::analogPinJoyX);
	        vertical = analogRead(Pins::analogPinJoyY);
            select = digitalRead(Pins::digitalPinJoySelect);            
        }
        void beep(){
            for (unsigned long go = millis(); millis() - go < 100; analogWrite(Pins::digitalPinSountOut, 50));
            digitalWrite(Pins::digitalPinSountOut, LOW);
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

namespace probing{

    struct readingsType{
        float ph;
        float temp;
        bool waterLevel;
    } readings{7,25,false};

    float readPH()
    {
        
    }

    float readTemp()
    {
        
    }

    float readWaterLevel()
    {
        return true;
    }

    void runProbbingLoop()
    {

    }
}

namespace control{

    uint32_t nextUpdateProbe = 0;
    
    void ph(float ph){

    }

    void refill(bool waterLevel)
    {
        //iconsIndicator->refill = waterLevel;
        screen::iconsIndicator.refill = waterLevel;
        if (waterLevel){
             uint16_t i=1;
        }
        else {
            uint16_t i=0;
        }
    }

    void runLoop()
    {
        if (millis() - nextUpdateProbe > 500)
        {
            nextUpdateProbe = millis();      
            float temperature = probing::readTemp();
            float ph = probing::readPH();
            refill(probing::readWaterLevel());            
        }
    }
}

struct SettingsType
{
    struct PhSettingsType
    {
        float up;
        float down;
        uint16_t interval;
        uint16_t onTime;
    } phSettings{6.7f, 6.5f, 300, 3};

    struct TemptSettingsType
    {
        float up;
        float down;
    }tempSettings{27.0f,25.5f};

    struct PhCalibrationSettingsType
    {
        float ph7V;
        float ph4V;
        float phFactor;
    }phCalibrationSettings {0.00f,0.00f,0.00f};

    struct ScreenSettingsType
    {
        uint16_t barLenght;        
    } screenSettings{13};

}settings;


class MainScreen
{
    
    enum ReadScreens: uint16_t{
        tempScreen=1,
        phScreen=2,
        carouselScreen=3
    };

    uint16_t m_currentBarPozition = settings.screenSettings.barLenght;
    uint32_t m_nextUpdateRead = 0;
    uint32_t m_nextUpdateJoystick = 0;
    char m_readType = 'P';
    uint16_t m_screenState = ReadScreens::carouselScreen;        
 
    

    public:
 
    void render()
    {
        if (millis() - m_nextUpdateRead > 1000)
        {   
            m_nextUpdateRead = millis();
            switch(m_screenState)
            {
                case ReadScreens::tempScreen:
                    screen::showReadings(m_currentBarPozition,"Temp",26.7f);
                    break;
                case ReadScreens::phScreen:
                    screen::showReadings(m_currentBarPozition,"Ph",7.5f);
                    break;
                case ReadScreens::carouselScreen:
                    m_currentBarPozition--; 
                    switch (m_readType)
                    {
                        case 'P':
                            screen::showReadings(m_currentBarPozition,"Ph",m_currentBarPozition, true);
                            break;
                        case 'T':
                            screen::showReadings(m_currentBarPozition,"Temp",m_currentBarPozition, true);        
                            break;
                    }
            
                    if  (m_currentBarPozition==0) {
                        m_currentBarPozition=settings.screenSettings.barLenght;
                        if (m_readType == 'T') {m_readType = 'P';} else {m_readType = 'T';}
                        }
                    break;
            }
 
        }  
    }
 
    void control(char joyState)
    {
        if (millis() - m_nextUpdateJoystick > 100)
        {
            m_nextUpdateJoystick = millis();       
            switch (joyState)
            {
                case 'l':           
                m_screenState--;
                if (m_screenState < ReadScreens::tempScreen) { m_screenState= ReadScreens::carouselScreen;}
                break;
                case 'r':
                m_screenState++;
                if (m_screenState > ReadScreens::carouselScreen) { m_screenState = ReadScreens::tempScreen;}
                break;
                case 'p':
                setNextScreen(ScreenType::settingsScreen);
                break;
            }
        }
    }
};
 
 
class OptionsScreen
{
    enum SettingsScreens : uint16_t{
    setTempUp = 1, 
    setTempDown = 2, 
    setPhDUp = 3,
    setPhDown = 4,
    setPhTime = 5 ,
    setPhPeriod = 6
    };

    uint16_t m_currentBarPozition = settings->screenSettings.barLenght;
    
    uint32_t m_nextUpdateRead = 0;
    uint32_t m_nextUpdateJoystick = 0;
    uint16_t m_screenState = SettingsScreens::setTempUp;    
    SettingsType *settings;
    

    void changeSetting (uint16_t screenState,bool add=true, float grade=0.1f)
    {   
        switch(screenState)
        {
            case SettingsScreens::setTempUp:
                if (add){settings->tempSettings.up=settings->tempSettings.up+grade;} else {settings->tempSettings.up=settings->tempSettings.up-grade;}
            break;
            case SettingsScreens::setTempDown:
            
            break;
            case SettingsScreens::setPhDUp:
            
            break;
            case SettingsScreens::setPhDown:
            
            break;
            case SettingsScreens::setPhTime:
            
            break;
            case SettingsScreens::setPhPeriod:
            
            break;
        }
    }

    public:
    
    OptionsScreen(SettingsType& _settings)
    {
        settings = &_settings;        
    }
    


    void render()
    {    
            switch(m_screenState)
            {
                case SettingsScreens::setTempUp:
                screen::showSettings(35,20,"Gor temp (C):",settings->tempSettings.up);
                break;
                case SettingsScreens::setTempDown:
                screen::showSettings(35,20,"Dol temp (C):",1);
                break;
                case SettingsScreens::setPhDUp:
                screen::showSettings(0,14,"Gor ph:",1);
                break;
                case SettingsScreens::setPhDown:
                screen::showSettings(0,14,"Dol ph:",1);
                break;
                case SettingsScreens::setPhTime:
                screen::showSettings(0,60,"Czas ph (s):",1);
                break;
                case SettingsScreens::setPhPeriod:
                screen::showSettings(0,20,"Okres ph (m):",1);
                break;
            }
    }

 
    void control(char joyState)
    {
        if (millis() - m_nextUpdateJoystick > 100)
        {
            
            m_nextUpdateJoystick = millis();       
            switch (joyState)
            {
                case 'l':           
                m_screenState--;
                if (m_screenState < SettingsScreens::setTempUp) { m_screenState= SettingsScreens::setPhPeriod;}
                break;
                case 'r':
                m_screenState++;
                if (m_screenState > SettingsScreens::setPhPeriod) { m_screenState = SettingsScreens::setTempUp;}
                break;
                case 'u':
                    changeSetting(m_screenState);
                break;
                case 'd':
                    changeSetting(m_screenState,false);
                break;
                case 'p':
                setNextScreen(ScreenType::saveScreen);
                break;
            }
        }
    }
};

class SaveScreen
{
    public:
    void render()
    {
        screen::showSave();
    }

    void control(SettingsType settings)
    {
         EEPROM.put(0, settings);  
         setNextScreen(ScreenType::mainScreen);       
    }
};

 
MainScreen screenMain;
OptionsScreen screenOptions (settings);
SaveScreen screenSave;
JoystickType joystick;



void setup()
{
    screen::initializeScreen();
    screen::showLogo();
    Serial.begin(9600);
    EEPROM.get( 0, settings);
}

void loop()
{ 

    switch(g_currentScreen)
    {
        case mainScreen:
            screenMain.control(joystick.readState());
            screenMain.render();
            break;
 
        case settingsScreen:
            screenOptions.control(joystick.readState());
            screenOptions.render();
            break;
         
        case saveScreen:
            screenSave.control(settings);
            screenSave.render();
            break;    
    }

    if ( g_currentScreen != settingsScreen)
    {
       //Serial.println(screen::iconsIndicator.refill);
       control::runLoop();
    }

    if (g_currentScreen != g_nextScreen)
    {
        g_currentScreen = g_nextScreen;
    }

}