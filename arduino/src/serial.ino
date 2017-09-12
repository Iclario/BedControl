#include "arduino.h"

void initSerial ()
{
	Serial.begin (DEBUG_BAUD_RATE); // Initialize debug communication
	Serial1.begin (ESP_BAUD_RATE);  // Initialize ESP8266 communication
	Serial2.begin (DFP_BAUD_RATE);  // Initialize DFPlayer communication
}

void waitForTime ()
{
	if (!WAIT_FOR_TIME)
		return;

	// Gently ask for time
	time_t lastMillis = millis();
	boolean flag      = false;

	while (timeStatus() == timeNotSet || !flag) // Doesn't start if time isn't set and we didn't receive all prayer times
	{
		readSerial();

		flag = true;

		for (int i = 0; i < N_PRAYER; i++)
			if (prayerTime[i][2] == 0)
				flag = false;

		if (millis() - lastMillis >= 5000)
		{
			printlnNoPrefix();
			if (timeStatus() == timeNotSet && !flag)
				println ("Neither time nor prayers are set");
			else if (timeStatus() == timeNotSet)
				println ("Time is not set");
			else
				println ("Prayers are not set");

			askForTime();
			lastMillis = millis();
		}
	}
} // waitForTime

// Asking for time to the ESP8266 (via internet)
void askForTime ()
{
	println ("Gently asking for time");
	Serial1.print ("TIMEPLEASEz"); // z is the end character
}

// Receive datas from ESP8266 for Wi-Wi control
void readSerial ()
{
	long result;
	int infoMode, infoType, errorType;

	if (!Serial.available() && !Serial1.available())
		return;  // Waiting for incomming datas

	decodeRequest (Serial.available() ? Serial.readStringUntil ('z') : Serial1.readStringUntil ('z'), &result, &infoMode, &infoType, &errorType);

	if (infoType == TYPE_RTM)
	{
		println ("I don't know anything about time... Let me ask the ESP");
		askForTime();
	}
	else if (infoType == TYPE_RIF)
		sendInfo();  // We send the variables values to the ESP8266
} // readSerial