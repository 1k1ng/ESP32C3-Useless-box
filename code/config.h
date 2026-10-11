// Platform types and constants.
#include <Arduino.h>

const char* APP_VERSION = "0.0.1";

#if defined(ARDUINO_ARCH_ESP32) && defined(CONFIG_IDF_TARGET_ESP32C3)
#ifndef LED_BUILTIN
#define LED_BUILTIN 8  // On most ESP32-C3 dev kits (e.g. DevKitM-1/02, LOLIN C3 Mini)
#endif

const int PIN_LID_SERVO = 4;      // PWM-capable, no boot strapping conflicts.
const int PIN_SWITCH_SERVO = 5;   // PWM-capable, no boot strapping conflicts.
const int PIN_SWITCH = 6;         // Free GPIO; keep switch wiring with external resistor to avoid floating.
const int PIN_SENSOR_SDA = 8;     // SDA wired to GPIO8 (boot strapping pin: unplug sensor for flashing).
const int PIN_SENSOR_SCL = 9;     // SCL wired to GPIO9 (boot strapping pin: unplug sensor for flashing).

#else  // ESP8266 (Wemos D1 mini) defaults
#ifndef LED_BUILTIN
#define LED_BUILTIN D4
#endif

const int PIN_LID_SERVO = 8;     // The GPIO number of the pin where the lid opener servo is connected.
const int PIN_SWITCH_SERVO = 0;  // The GPIO number of the pin where the switch manipulator servo is connected.
const int PIN_SWITCH = 5;        // The GPIO number of the pin where the switch is connected.
const int PIN_SENSOR_SDA = 8;    // The GPIO number of the pin where the SDA pin of the APDS-9660 sensor is connected.
const int PIN_SENSOR_SCL = 9;    // The GPIO number of the pin where the SCL pin of the APDS-9660 sensor is connected.
#endif

const uint8_t SENSOR_TRIGGER_THRESHOLD = 50;  // The proximity value level that triggers a lid opening. (lower=brighter, higher=darker)

const int LID_START_POSITION = 90;
const int LID_END_POSITION = 46;  // Lower = lid opens wider.
const int SWITCH_START_POSITION = 0;
const int SWITCH_END_POSITION = 180;  // Max travel/power to fully flip the switch.
const int SWITCH_HALF_POSITION = 90;
