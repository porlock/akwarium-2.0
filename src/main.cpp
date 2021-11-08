#include <Arduino.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <EEPROM.h>
#include "FixedPoint.h"
#include "GlobalEnums.h"
#include "ScreenManager.h"
#include "screen.h"
#include "SettingsType.h"
 
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


class JoystickType
{
    private:
        uint16_t horizontal, vertical, select;
        JoyStatus status = JoyStatus::eCenter;
        JoyStatus prevStatus = JoyStatus::eCenter;
        void readPins(){
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
        UF16x2 ph;
        UF16x2 temp;
        bool waterLevel;
    } readings{7,25,false};

    UF16x2 readPH()
    {
        
    }

    UF16x2 readTemp()
    {
        
    }

    bool readWaterLevel()
    {
        return true;
    }

    void runProbbingLoop()
    {

    }
}

namespace control{

    uint32_t nextUpdateProbe = 0;
    
    void ph(UF16x2 ph){

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
            UF16x2 temperature = probing::readTemp();
            UF16x2 ph = probing::readPH();
            refill(probing::readWaterLevel());            
        }
    }
}

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

            switch(m_screenState)
            {
                case ReadScreens::eScreenReadTemp:
                    screen::showReadings(m_currentBarPosition,"Temp",27.5);
                    break;
                case ReadScreens::eScreenReadPh:
                    screen::showReadings(m_currentBarPosition,"Ph",7.5);
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

    // m_settings jest referencja typu SettingsType
    const SettingsType &m_settings;

    /* 
    m_settings(_settings)
    to jest lista inicjalizacyjna która się odpala przed kodem konstruktora, 
    dzięki temu możesz przypisać referencję w trakcie tworzenia instancji klasy
    */
    MainScreen(const SettingsType& _settings) : m_settings(_settings)
    {
    }

    void init()
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
        eSetTempDown = eSetBegin, 
        eSetTempUp = 2, 
        eSetPhDown = 3,
        eSetPhUp = 4,
        eSetPhTime = 5 ,
        eSetPhPeriod = 6,
        eSetSave = 7,
        eSetDefaults = 8,
        eSetPhCalibration = 9,    
        eSetEnd
    };

    
    enum SettingsStatusScreens : uint16_t {
        eOptionsSave,
        eOptionsDefault        
    };

   
    uint32_t m_nextUpdateRead = 0;
    uint32_t m_nextUpdateJoystick = 0;
    uint16_t m_screenState = SettingsScreens::eSetBegin;
    uint16_t m_screenStatusState;
    SettingsType& m_settings;
    
    struct {
        UF16x2 phGrade = 0.1f;
        UF16x2 tempGrade = 0.5f;
        uint16_t timeGrade = 1;
        uint16_t intevalGrade = 1;
    } settingsGrades;

    struct {
        UF16x2 phMax = 8;
        UF16x2 phMin = 6;
        UF16x2 tempMin = 20;
        UF16x2 tempMax= 35;
        uint16_t phIntervalMax = 60;
        uint16_t phIntervalMin = 1;
        uint16_t phTimeMax = 30;
        uint16_t phTimeMin = 1;
    } settingsLimits;

    void changeSetting (uint16_t screenState,bool add=true)
    {   
        switch(screenState)
        {
            case SettingsScreens::eSetTempDown:
                if (add && m_settings.tempSettings.down < settingsLimits.tempMax){
                    m_settings.tempSettings.down+=settingsGrades.tempGrade;} 
                else if (!add && m_settings.tempSettings.down > settingsLimits.tempMin){
                    m_settings.tempSettings.down-=settingsGrades.tempGrade;}
                break;
            case SettingsScreens::eSetTempUp:                
                if (add && m_settings.tempSettings.up < settingsLimits.tempMax){
                    m_settings.tempSettings.up+=settingsGrades.tempGrade;} 
                else if (!add && m_settings.tempSettings.up > settingsLimits.tempMin) {
                    m_settings.tempSettings.up-=settingsGrades.tempGrade;}
                break;
            case SettingsScreens::eSetPhDown:
                if (add && m_settings.phSettings.down < settingsLimits.phMax){
                    m_settings.phSettings.down+=settingsGrades.phGrade;}
                else if (!add && m_settings.phSettings.down > settingsLimits.phMin) {
                    m_settings.phSettings.down-=settingsGrades.phGrade;}
                break;
            case SettingsScreens::eSetPhUp:
                if (add && m_settings.phSettings.up < settingsLimits.phMax){
                    m_settings.phSettings.up+=settingsGrades.phGrade;} 
                else if (!add &&  m_settings.phSettings.up > settingsLimits.phMin){
                    m_settings.phSettings.up-=settingsGrades.phGrade;}
                break;
            case SettingsScreens::eSetPhTime:
                if (add && m_settings.phSettings.onTime < settingsLimits.phTimeMax){
                    m_settings.phSettings.onTime+=settingsGrades.timeGrade;} 
                else if (!add && m_settings.phSettings.onTime > settingsLimits.phTimeMin) {
                    m_settings.phSettings.onTime-=settingsGrades.timeGrade;}
                break;
            case SettingsScreens::eSetPhPeriod:
                if (add && m_settings.phSettings.interval < settingsLimits.phIntervalMax){
                    m_settings.phSettings.interval+=settingsGrades.intevalGrade;} 
                else if (!add && m_settings.phSettings.interval > settingsLimits.phIntervalMin) {
                    m_settings.phSettings.interval-=settingsGrades.intevalGrade;}
                break;
        }
    }

    public:
    // _settings jest referencja typu SettingsType
    OptionsScreen(SettingsType& _settings) : m_settings(_settings)
    {    
    }
    
    void init()
    {
    }

    void render()
    {
        if (displaySettingScreens){
            switch(m_screenState)
            {
                case SettingsScreens::eSetTempUp:
                    screen::showSettings(max(settingsLimits.tempMin,m_settings.tempSettings.down),settingsLimits.tempMax,"Gor temp (C):",m_settings.tempSettings.up);
                    break;
                case SettingsScreens::eSetTempDown:
                    screen::showSettings(settingsLimits.tempMin,settingsLimits.tempMax,"Dol temp (C):",m_settings.tempSettings.down);
                    break;
                case SettingsScreens::eSetPhUp:
                    screen::showSettings(settingsLimits.phMin,settingsLimits.phMax,"Gor ph:",m_settings.phSettings.up);
                    break;
                case SettingsScreens::eSetPhDown:
                    screen::showSettings(settingsLimits.phMin,settingsLimits.phMax,"Dol ph:",m_settings.phSettings.down);
                    break;
                case SettingsScreens::eSetPhTime:
                    screen::showSettings(settingsLimits.phTimeMin,settingsLimits.phTimeMax,"Czas ph (s):",m_settings.phSettings.onTime);
                    break;
                case SettingsScreens::eSetPhPeriod:
                    screen::showSettings(settingsLimits.phIntervalMin,settingsLimits.phIntervalMax,"Okres ph (m):",m_settings.phSettings.interval);
                    break;
                case SettingsScreens::eSetSave:
                    screen::showClickOption("Zapis ustawien");
                    break;
                case SettingsScreens::eSetDefaults:
                    screen::showClickOption("Reset ustawien");
                    break;
                case SettingsScreens::eSetPhCalibration:
                    screen::showClickOption("Kalibracja PH");
                    break;
            }
        }
        else {
                 switch(m_screenStatusState){
                     case SettingsStatusScreens::eOptionsSave:
                        screen::showClickOption("Zapisane",1,1500);
                     break;
                     case SettingsStatusScreens::eOptionsDefault:
                        screen::showClickOption("Przywrocono",1,1500);
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
                    switch (m_screenState)
                    {
                        case SettingsScreens::eSetSave:
                            m_screenStatusState = SettingsStatusScreens::eOptionsSave;
                            m_screenState=SettingsScreens::eSetBegin;
                            EEPROM.put( 0, settings);
                            displaySettingScreens=false;
                            break;
                        case SettingsScreens::eSetDefaults:
                            m_screenStatusState = SettingsStatusScreens::eOptionsDefault;
                            m_screenState=SettingsScreens::eSetBegin;
                            //defreferencja, zapisanie w miesjscu na ktore wskasuje wkaźnik
                            //*settingsPointer = SettingsType(); 
                            m_settings = defaultSettings;
                            displaySettingScreens = false;
                            break;
                        case SettingsScreens::eSetPhCalibration:
                            setNextScreen(ScreenType::eScreenPhCalibraton);
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

class PhCalibrationScreen{
    uint16_t i = 0;
    public:
    void startPhCalibration()
    {
      
        if (i < 10){
            screen::showClickOption("Umiesc sade w ph4",1,500);
            i++;
        }
        else {
            i=0;
            setNextScreen(ScreenType::eScreenMain);
        }
    }
};

MainScreen screenMain(settings);
OptionsScreen screenOptions(settings);
PhCalibrationScreen screenPhCalibraton;

JoystickType joystick;

void setup()
{
    screen::initializeScreen();
    screen::showLogo();
    Serial.begin(9600);
    Serial.println("Program Start");
    EEPROM.get( 0, settings);

    screenMain.init();
    screenOptions.init();
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
        case eScreenPhCalibraton:
            screenPhCalibraton.startPhCalibration();
            break;
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