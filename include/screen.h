#pragma once
#include <stdint.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include "FixedPoint.h"
#include "GlobalEnums.h"

namespace screen
{
	struct IconsIndicatorsType
	{
		bool refill;
		bool hcl;
		bool heater;
		bool error;
	};
	// extern bo użyte po za tym plikiem
	extern IconsIndicatorsType iconsIndicator;

	void initializeScreen();
	void clearScreen();
	void showLogo();
	void showReadings(uint16_t, const char *, UF16x2, bool = false);
	void showSettings(UF16x2, UF16x2, const char *, UF16x2);
	void showSettings(uint16_t, uint16_t, const char *, uint16_t);
	void showClickOption(const char *, uint16_t = 1, uint16_t = 0);
	void showCalibrationInfo(UF16x2 ph7V, UF16x2 ph4V, UF16x2 phFactor);
	void showCalibration(CalibrationPhase phase, UF16x2 voltage = 0, bool blink = false);
	void showMemmoryError();
	void showSave();
}