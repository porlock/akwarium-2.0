#include <Arduino.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <EEPROM.h>


enum Pins : uint16_t{
    eDigitalPinSoundOut=6,
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
    eScreenMain,
    eScreenSettings,
    eScreenSave,
};

ScreenType g_currentScreen = eScreenMain;
ScreenType g_nextScreen = eScreenMain;

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
		display.print("od "); display.print(minValue); display.print(" do "); display.println(maxValue);
        display.println();
		display.setTextSize(2);
		display.println(value);   
        display.display(); 
    }

    void showClickOption(const char* text, uint16_t fontSize=1, uint16_t pause=0)
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
            analogWrite(Pins::eDigitalPinSoundOut, 50);
            delay(100);
            digitalWrite(Pins::eDigitalPinSoundOut, LOW);
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

    struct ReadingsType{
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
    }phSettings;

    struct TempSettingsType
    {
        float up;
        float down;
    }tempSettings;

    struct PhCalibrationSettingsType
    {
        float ph7V;
        float ph4V;
        float phFactor;
    }phCalibrationSettings;

    struct ScreenSettingsType
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
        eScreenReadBegin=1,
        eScreenReadTemp=eScreenReadBegin,
        eScreenReadPh=2,
        eScreenReadCorousel =3,
        eScreenReadEnd
    };
    
    uint16_t m_currentBarPosition = 0;
    uint16_t m_maxBarPosition=0;
    uint32_t m_nextUpdateRead = 0;
    uint32_t m_nextUpdateJoystick = 0;
    bool skip=true;
    char m_readType = 'P';
    uint16_t m_screenState = ReadScreens::eScreenReadCorousel;        

    public:
 
    void render()
    {
        if (millis() - m_nextUpdateRead > 500)
        {   
            m_nextUpdateRead = millis();

            //Serial.println(m_screenState);
            switch(m_screenState)
            {
                case ReadScreens::eScreenReadTemp:
                    screen::showReadings(m_currentBarPosition,"Temp",26.7f);
                    break;
                case ReadScreens::eScreenReadPh:
                    screen::showReadings(m_currentBarPosition,"Ph",7.5f);
                    break;
                case ReadScreens::eScreenReadCorousel:
                    if (skip) { m_currentBarPosition--; skip=false;} else {skip=true;}
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
                if (m_screenState < ReadScreens::eScreenReadBegin) { m_screenState = ReadScreens::eScreenReadEnd -1;}
                break;
                case JoyStatus::eRight:
                m_screenState++;
                if (m_screenState >= ReadScreens::eScreenReadEnd) { m_screenState = ReadScreens::eScreenReadBegin;}
                break;
                case JoyStatus::eSelect:
                setNextScreen(ScreenType::eScreenSettings);
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
    bool displaySettingScreens = true;
    enum SettingsScreens : uint16_t{
    eSetBegin = 1,
    eSetTempUp = eSetBegin, 
    eSetTempDown = 2, 
    eSetPhUp = 3,
    eSetPhDown = 4,
    eSetPhTime = 5 ,
    eSetPhPeriod = 6,
    eSetSave = 7,
    eSetDefaults = 8,    
    eSetEnd
    };

    enum SettingsStatusScreens : uint16_t {
        eOptionsSave,
        eOptionsDefault        
    };
    
    uint32_t m_nextUpdateRead = 0;
    uint32_t m_nextUpdateJoystick = 0;
    uint16_t m_screenState = SettingsScreens::eSetTempUp;
    uint16_t m_screenStatusState;
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
            case SettingsScreens::eSetPhUp:
            
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
        Serial.println(displaySettingScreens);    
        if (displaySettingScreens){
            switch(m_screenState)
            {
                case SettingsScreens::eSetTempUp:
                    screen::showSettings(20,35,"Gor temp (C):",settingsPointer->tempSettings.up);
                    break;
                case SettingsScreens::eSetTempDown:
                    screen::showSettings(20,35,"Dol temp (C):",1);
                    break;
                case SettingsScreens::eSetPhUp:
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
                case SettingsScreens::eSetSave:
                    screen::showClickOption("Zapisz ustawienia");
                    break;
                case SettingsScreens::eSetDefaults:
                    screen::showClickOption("Reset ustawien");
                    break;    
            }
        }
        else {
                 switch(m_screenStatusState){
                     case SettingsStatusScreens::eOptionsSave:
                        screen::showClickOption("Zapisane",1,1500);
                     break;
                     case SettingsStatusScreens::eOptionsDefault:
                        screen::showClickOption("Przywrócono",1,1500);
                }
            displaySettingScreens=true;
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
                    if (m_screenState < SettingsScreens::eSetBegin) { m_screenState= SettingsScreens::eSetEnd -1 ;}
                    break;
                case JoyStatus::eRight:
                    m_screenState++;
                    if (m_screenState >= SettingsScreens::eSetEnd) { m_screenState = SettingsScreens::eSetBegin;}
                    break;
                case JoyStatus::eUp:
                    changeSetting(m_screenState);
                    break;
                case JoyStatus::eDown:
                    changeSetting(m_screenState,false);
                    break;
                case JoyStatus::eSelect:
                    Serial.println("select");
                    switch (m_screenState)
                    {
                        case OptionsScreen::eSetSave:
                            Serial.println("eSetSave");
                            m_screenStatusState = SettingsStatusScreens::eOptionsSave;
                            m_screenState=SettingsScreens::eSetBegin;
                            //tu zapis ustawień
                            displaySettingScreens=false;
                            
                            break;
                        case OptionsScreen::eSetDefaults:
                            m_screenStatusState = SettingsStatusScreens::eOptionsDefault;
                            m_screenState=SettingsScreens::eSetBegin;
                            //tu reset ustawień
                            displaySettingScreens=false;
                            break;
                        default:
                            setNextScreen(ScreenType::eScreenMain);
                            break;
                    }
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
         setNextScreen(ScreenType::eScreenMain);       
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
    Serial.println("Program Start");
    //EEPROM.get( 0, settings);
}

void loop()
{ 
     switch(g_currentScreen)
    {
        case eScreenMain:
            screenMain.control(joystick.readState());
            screenMain.render();
            break;
 
        case eScreenSettings:
            screenOptions.control(joystick.readState());
            screenOptions.render();
            break;
         
       /* case eScreenSave:
            screenSave.control(settings);
            screenSave.render();
            break;    
        */
    }

    if ( g_currentScreen != eScreenSettings)
    {
       control::runLoop();
    }

    if (g_currentScreen != g_nextScreen)
    {
        g_currentScreen = g_nextScreen;
    }

}