# Akwarium 2.0

An aquarium controller built on **Arduino Nano (ATmega328)**. It measures water temperature and pH, displays readings on a Nokia 5110 LCD, and provides a joystick-operated settings menu.

The project brings basic aquarium monitoring and control into a single device: checking water parameters, controlling a heater, and automating pH reduction and water top-ups. It runs locally without an internet connection. Some automation features still need work, as described below.

## Features

- **Temperature monitoring** — reads the first sensor on the OneWire bus using DallasTemperature.
- **pH monitoring** — converts the analog probe module's voltage into pH, smoothing readings with a 20-sample moving average.
- **LCD dashboard** — displays temperature, pH, or alternating readings, along with control status and error icons.
- **Heater control** — switches a relay output according to water temperature.
- **Joystick configuration** — provides temperature and pH thresholds, dosing pulse duration, and pulse interval settings.
- **pH calibration** — includes a two-point calibration procedure using pH 7 and pH 4 buffers.
- **Persistent settings** — saves configuration to EEPROM and loads it after a restart. If the stored data marker does not match, the controller uses its defaults.

## Automation status

This is a work in progress. The current implementation has these limitations:

| Area | Current behavior |
| --- | --- |
| Heater | Turns on below the lower threshold and off above that same threshold. The upper threshold is available in the menu but is not used by the control logic, so two-threshold hysteresis is not implemented. |
| pH reduction | The output named `HCL` is intended to dose periodically after pH exceeds the upper threshold, until it drops below the lower threshold. However, `RelayType::setClockOn()` contains the declaration `void setOn();` instead of the call `setOn();`, so the pulse does not activate the output. |
| Water top-up | A relay output and control logic exist, but `readWaterLevel()` always sets `false`. Water-level sensor input is not implemented. |
| Calibration | While the calibration screen is active, the main measurement and control loop is skipped. Previously active outputs are not automatically switched off. |
| Temperature error | A missing sensor at startup triggers an error indication and sets the reading to 0°C. This does not inhibit heater control. |

Before using the automation in an operating aquarium, address these limitations and verify output behavior and fault handling on the hardware.

## Hardware

- Arduino Nano with ATmega328, matching the `nanoatmega328` configuration.
- Nokia 5110 LCD with a PCD8544 controller.
- A temperature sensor compatible with OneWire and DallasTemperature, such as a DS18B20.
- A pH probe with a module providing an analog voltage output.
- An analog joystick with a push button and a sound indicator.
- Suitable actuator modules for the heater, dosing device, and planned water top-up system.

Signal assignments in the code:

| Signal | Nano pin |
| --- | --- |
| OneWire temperature sensor | D4 |
| Analog pH input | A6 |
| Joystick X / Y | A0 / A1 |
| Joystick button | D12 |
| Sound indicator | D6 |
| `HCL` output | D2 |
| Heater output | D3, active-low logic |
| Water top-up output | D5 |

The display is initialized in `src/screen.cpp` with `Adafruit_PCD8544(11, 10, 9, 8)`. Before wiring it, check the constructor arguments for the library version in use. The table above documents software signal assignments, not a complete wiring and power schematic.

## Build and upload

The project uses **PlatformIO** with the Arduino framework. Dependencies and versions are declared in `platformio.ini`: Adafruit GFX, Adafruit PCD8544, OneWire, and DallasTemperature.

1. Open the project folder in PlatformIO, for example in Visual Studio Code with the PlatformIO IDE extension.
2. Connect the Arduino Nano over USB.
3. Build and upload using the PlatformIO interface or terminal:

   ```sh
   pio run -e nanoatmega328
   pio run -e nanoatmega328 --target upload
   ```

4. Optionally open the serial monitor:

   ```sh
   pio device monitor --baud 9600
   ```

## Controls

On the main screen, move the joystick left or right to switch between reading views. Press the button to open settings.

In the settings menu:

- **Left / right** — select the previous or next item.
- **Up / down** — change the value.
- **Button** — perform the selected action (save, reset, or calibrate), or return to the main screen when a numeric setting is selected.

Setting changes apply to the current session. To retain them after power is disconnected, select **"Zapis ustawien" (Save settings)**. This also applies after calibration or restoring defaults. The device's menu labels are currently in Polish.

Calibration proceeds through pH 7 and pH 4 buffer measurements, waiting for the voltage to stabilize. The code expects 2.9–3.1 V for pH 7 and 3.3–3.5 V for pH 4, so the procedure is tailored to a particular measurement circuit. The `vRef` value in `include/SettingsType.h` also needs to match the hardware.

## Default settings

These are software defaults, not recommended parameters for a particular aquarium or its livestock.

| Parameter | Value |
| --- | --- |
| Lower / upper pH threshold | 6.5 / 6.7 |
| `HCL` pulse duration | 3 s |
| `HCL` pulse interval | 30 min |
| Lower / upper temperature threshold | 25 / 27°C |
| ADC conversion reference voltage (`vRef`) | 4.49 V |

## Code layout

- `src/main.cpp` — initialization, main loop, and control logic.
- `src/probing.cpp` — sensor readings and pH conversion.
- `src/RelayType.cpp` — relay output handling.
- `src/MainScreen.cpp`, `src/OptionsScreen.cpp`, `src/PhCalibrationScreen.cpp` — screens and their interaction logic.
- `src/screen.cpp` — LCD interface rendering.
- `include/SettingsType.h` — configuration structure and defaults.
- `include/GlobalEnums.h` — pin assignments and interface states.
- `platformio.ini` — target board, framework, and libraries.
