#include <Time.h>
#include "Logger.h"
#include "SdCard.h"
#include "VariableChange.h"
#include "ArduinoSerial.h"
#include "Alarms.h"
#include "Bluetooth.h"
#include "Light.h"
#include "Sound.h"
#include "ESPSerial.h"
#include "Wifi.h"
#include "Infrared.h"

const boolean WAIT_FOR_TIME = true; // If the Arduino has to wait for time sync (if true, program will not start until time is synced)

const long COMM_BAUD_RATE = 115200; // ESP8266/Arduino communication baud rate

#if defined(LUMOS_ARDUINO_MEGA) // Arduino code
void setup ()
{
	serial.init (ARDUINO_DEBUG_BAUD_RATE, COMM_BAUD_RATE);

	logger.add (serial.debugSerial, DEBUG_LEVEL);

	if (WAIT_FOR_TIME) serial.waitForTime();

	sd.init();

	light.init();

	infrared.init();

	sound.init (Serial3);

	alarms.init();

	variableChange.init();

	variableChange.sendInfo();
}

void loop ()
{
	variableChange.check();

	serial.receiveAndDecode();

	sd.action();

	infrared.read();

	light.action();

	sound.action();

	alarms.action();

	// bluetooth.action();
}

#endif // if defined(LUMOS_ARDUINO_MEGA)

#if defined(LUMOS_ESP8266) // ESP8266 Code
void setup ()
{
	serial.init (ESP_DEBUG_BAUD_RATE, COMM_BAUD_RATE);

	logger.add (serial.debugSerial, DEBUG_LEVEL);

	wifi.init();

	wifi.getTime();

	serial.sendTime();

	serial.getInfos();
}

void loop ()
{
	serial.receiveAndDecode();

	wifi.receiveAndDecode();
}

#endif // if defined(LUMOS_ESP8266)