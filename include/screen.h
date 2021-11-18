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
	void showReadings(uint16_t, const char *, F16x3, bool = false);
	void showSettings(F16x3, F16x3, const char *, F16x3);
	void showSettings(uint16_t, uint16_t, const char *, uint16_t);
	void showClickOption(const char *, uint16_t = 1, uint16_t = 0);
	void showCalibrationInfo(F16x3 ph7V, F16x3 ph4V, F16x3 phFactor);
	void showCalibration(CalibrationPhase phase, F16x3 voltage = 0, bool blink = false);
	void showMemmoryError();
	void showSave();
}