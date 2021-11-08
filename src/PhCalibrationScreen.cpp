#include <PhCalibrationScreen.h>

PhCalibrationScreen::PhCalibrationScreen()
{
    i = 0;
}

void PhCalibrationScreen::startPhCalibration()
{

    if (i < 10)
    {
        screen::showClickOption("Umiesc sade w ph4", 1, 500);
        i++;
    }
    else
    {
        i = 0;
        setNextScreen(ScreenType::eScreenMain);
    }
}
