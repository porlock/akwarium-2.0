#pragma once
#include <stdint.h>
#include "GlobalEnums.h"

ScreenType g_currentScreen = eScreenMain;
ScreenType g_nextScreen = eScreenMain;

void setNextScreen(ScreenType screen){ g_nextScreen = screen;}