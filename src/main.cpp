#include <Arduino.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <EEPROM.h>


enum Pins : uint16_t{
    eDigitalPinSountOut=6,
    eDigitalPinTempIn=4,
    eDigitalPinJoySelect=12,
    eAnalogPinJoyX=0,
    eAnalogPinJoyY=1
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


enum JoyStatus
{
    eUp,
    eDown,
    eLeft,
    eRight,
    eCenter,
    eSelect
};

enum ScreenType
{
    eMainScreen,
    eSettingsScreen,
    eSaveScreen,
};

ScreenType g_currentScreen = eMainScreen;
ScreenType g_nextScreen = eMainScreen;

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


    void showSettings(uint16_t minValue, uint16_t maxValue, const char* name, float value)
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
        JoyStatus status = JoyStatus::eCenter;
        JoyStatus prevStatus = JoyStatus::eCenter;
        void readPins()
        {
            horizontal = analogRead(Pins::eAnalogPinJoyX);
	        vertical = analogRead(Pins::eAnalogPinJoyY);
            select = digitalRead(Pins::eDigitalPinJoySelect);            
        }
        void beep(){
            for (unsigned long go = millis(); millis() - go < 100; analogWrite(Pins::eDigitalPinSountOut, 50));
            digitalWrite(Pins::eDigitalPinSountOut, LOW);
        }
   
    public:
        JoyStatus readState()
        {
            readPins();
            if (!select) {status = JoyStatus::eSelect; }
            else if (horizontal >700 ) {status = JoyStatus::eLeft; }
            else if (horizontal <300) {status = JoyStatus::eRight; }
            else if (vertical > 700) {status = JoyStatus::eDown;}
            else if (vertical < 300) {status = JoyStatus::eUp;}
            else {status = JoyStatus::eCenter;}
          
            if (status == prevStatus){
                return JoyStatus::eCenter;                
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
    struct phSettingsType
    {
        float up;
        float down;
        uint16_t interval;
        uint16_t onTime;
    }phSettings;

    struct tempSettingsType
    {
        float up;
        float down;
    }tempSettings;

    struct phCalibrationSettingsType
    {
        float ph7V;
        float ph4V;
        float phFactor;
    }phCalibrationSettings;

    struct screenSettingsType
    {
        uint16_t barLenght;        
    }screenSettings;

}settings = {
    {6.7f, 6.5f, 300, 3},
    {27.0f,25.5f},
    {0.00f,0.00f,0.00f},
    {15}
};


class MainScreen
{
    
    enum ReadScreens: uint16_t{
        eReadBegin=1,
        eTempScreen=eReadBegin,
        ePhScreen=2,
        eCarouselScreen=3,
        eReadEnd
    };
    
    uint16_t m_currentBarPosition = 0;
    uint16_t m_maxBarPosition=0;
    uint32_t m_nextUpdateRead = 0;
    uint32_t m_nextUpdateJoystick = 0;
    char m_readType = 'P';
    uint16_t m_screenState = ReadScreens::eCarouselScreen;        

    public:
 
    void render()
    {
        if (millis() - m_nextUpdateRead > 1000)
        {   
            m_nextUpdateRead = millis();
            switch(m_screenState)
            {
                case ReadScreens::eTempScreen:
                    screen::showReadings(m_currentBarPosition,"Temp",26.7f);
                    break;
                case ReadScreens::ePhScreen:
                    screen::showReadings(m_currentBarPosition,"Ph",7.5f);
                    break;
                case ReadScreens::eCarouselScreen:
                    m_currentBarPosition--; 
                    switch (m_readType)
                    {
                        case 'P':
                            screen::showReadings(m_currentBarPosition,"Ph",m_currentBarPosition, true);
                            break;
                        case 'T':
                            screen::showReadings(m_currentBarPosition,"Temp",m_currentBarPosition, true);        
                            break;
                    }                
                    //Serial.println(m_currentBarPosition);                        
                    if  (m_currentBarPosition==0) {                             
                        m_currentBarPosition=m_maxBarPosition;
                            if (m_readType == 'T') {m_readType = 'P';} else {m_readType = 'T';}
                        }
                    break;
            }
        }  
    }
 
    void control(JoyStatus joyState)
    {
        if (millis() - m_nextUpdateJoystick > 100)
        {
            m_nextUpdateJoystick = millis();       
            switch (joyState)
            {
                case JoyStatus::eLeft:           
                m_screenState--;
                if (m_screenState < ReadScreens::eReadBegin) { m_screenState= ReadScreens::eReadEnd;}
                break;
                case JoyStatus::eRight:
                m_screenState++;
                if (m_screenState > ReadScreens::eReadEnd) { m_screenState = ReadScreens::eReadBegin;}
                break;
                case JoyStatus::eSelect:
                setNextScreen(ScreenType::eSettingsScreen);
                break;
            }
        }
    }


    const SettingsType &m_settings;
    
    MainScreen(const SettingsType& _settings) : m_settings(_settings)
    {
        m_currentBarPosition = m_settings.screenSettings.barLenght;
        m_maxBarPosition = m_currentBarPosition;
    }


};
 
 
class OptionsScreen
{
    enum SettingsScreens : uint16_t{
    eSetBegin = 1,
    eSetTempUp = eSetBegin, 
    eSetTempDown = 2, 
    eSetPhDUp = 3,
    eSetPhDown = 4,
    eSetPhTime = 5 ,
    eSetPhPeriod = 6,    
    eSetEnd
    };
    
    uint32_t m_nextUpdateRead = 0;
    uint32_t m_nextUpdateJoystick = 0;
    uint16_t m_screenState = SettingsScreens::eSetTempUp;    
    SettingsType *settingsPointer;
    

    void changeSetting (uint16_t screenState,bool add=true, float grade=0.1f)
    {   
        switch(screenState)
        {
            case SettingsScreens::eSetTempUp:
                if (add){settingsPointer->tempSettings.up+=grade;} else {settingsPointer->tempSettings.up-=grade;}
            break;
            case SettingsScreens::eSetTempDown:
            
            break;
            case SettingsScreens::eSetPhDUp:
            
            break;
            case SettingsScreens::eSetPhDown:
            
            break;
            case SettingsScreens::eSetPhTime:
            
            break;
            case SettingsScreens::eSetPhPeriod:
            
            break;
        }
    }

    public:
    
    OptionsScreen(SettingsType& _settings)
    {
        settingsPointer = &_settings;        
    }
    


    void render()
    {    
            switch(m_screenState)
            {
                case SettingsScreens::eSetTempUp:
                screen::showSettings(35,20,"Gor temp (C):",settingsPointer->tempSettings.up);
                break;
                case SettingsScreens::eSetTempDown:
                screen::showSettings(35,20,"Dol temp (C):",1);
                break;
                case SettingsScreens::eSetPhDUp:
                screen::showSettings(0,14,"Gor ph:",1);
                break;
                case SettingsScreens::eSetPhDown:
                screen::showSettings(0,14,"Dol ph:",1);
                break;
                case SettingsScreens::eSetPhTime:
                screen::showSettings(0,60,"Czas ph (s):",1);
                break;
                case SettingsScreens::eSetPhPeriod:
                screen::showSettings(0,20,"Okres ph (m):",1);
                break;
            }
    }

 
    void control(JoyStatus joyState)
    {
        if (millis() - m_nextUpdateJoystick > 100)
        {
            
            m_nextUpdateJoystick = millis();       
            switch (joyState)
            {
                case JoyStatus::eLeft:           
                m_screenState--;
                if (m_screenState < SettingsScreens::eSetBegin) { m_screenState= SettingsScreens::eSetEnd;}
                break;
                case JoyStatus::eRight:
                m_screenState++;
                if (m_screenState > SettingsScreens::eSetEnd) { m_screenState = SettingsScreens::eSetBegin;}
                break;
                case JoyStatus::eUp:
                    changeSetting(m_screenState);
                break;
                case JoyStatus::eDown:
                    changeSetting(m_screenState,false);
                break;
                case JoyStatus::eSelect:
                
                setNextScreen(ScreenType::eSaveScreen);
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
         //EEPROM.put(0, settings);  
         setNextScreen(ScreenType::eMainScreen);       
    }
};

 
MainScreen screenMain(settings);
OptionsScreen screenOptions(settings);
SaveScreen screenSave;
JoystickType joystick;



void setup()
{
    screen::initializeScreen();
    screen::showLogo();
    Serial.begin(9600);
    //EEPROM.get( 0, settings);
}

void loop()
{ 

    //Serial.println(settings.screenSettings.barLenght);
    switch(g_currentScreen)
    {
        case eMainScreen:
            screenMain.control(joystick.readState());
            screenMain.render();
            break;
 
        case eSettingsScreen:
            screenOptions.control(joystick.readState());
            screenOptions.render();
            break;
         
        case eSaveScreen:
            screenSave.control(settings);
            screenSave.render();
            break;    
    }

    if ( g_currentScreen != eSettingsScreen)
    {
       control::runLoop();
    }

    if (g_currentScreen != g_nextScreen)
    {
        g_currentScreen = g_nextScreen;
    }

}