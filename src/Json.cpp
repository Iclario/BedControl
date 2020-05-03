#include <Arduino.h>

#if defined(LUMOS_ESP8266)

# include "Json.h"
# include <ArduinoJson.h>
# include "Light.h"
# include "Sound.h"
# include "ArduinoLogger.h"
# include "Utils.h"
# include "Resources.h"

Json::Json()
{ }

String Json::getDataPretty ()
{
	String string;

	generateData (string, true);

	return string;
}

String Json::getData ()
{
	String string;

	generateData (string, false);

	return string;
}

void Json::generateData (String & string, bool pretty)
{
	const char status[]  = "OK";
	const char message[] = "";

	const size_t capacity =
	  JSON_OBJECT_SIZE (4)                 // root (status/message/light/sound)
	  + sizeof(status) + sizeof(message)   // status + message (Strings)
	  + JSON_OBJECT_SIZE (5)               // light (on/mode/rgb/power/speed)
	  + 3 * JSON_ARRAY_SIZE (LightMode::N) // light:rgb[] + light:power[] + light:speed[]
	  + JSON_OBJECT_SIZE (3)               // sound (on/volume/mode)
	  + 10                                 // Security margin
	;

	DynamicJsonDocument root (capacity);

	root["Status"]  = status;
	root["Message"] = message;

	// ****** Light ****** //
	JsonObject rootLight = root.createNestedObject ("Light");
	rootLight["On"]   = light.isOn();
	rootLight["Mode"] = (uint8_t) light.getMode();

	// -- Rgb -- //
	JsonArray rootLightRgb = rootLight.createNestedArray ("Rgb");
	for (LightMode mode = LightMode::MIN; mode <= LightMode::MAX; mode++)
		rootLightRgb.add (light.getRgb (mode).value());

	// -- Power -- //
	JsonArray rootLightPower = rootLight.createNestedArray ("Power");
	for (LightMode mode = LightMode::MIN; mode <= LightMode::MAX; mode++)
		rootLightPower.add (light.getPowerPercent (mode).value());

	// -- Speed -- //
	JsonArray rootLightSpeed = rootLight.createNestedArray ("Speed");
	for (LightMode mode = LightMode::MIN; mode <= LightMode::MAX; mode++)
		rootLightSpeed.add (light.getSpeedPercent (mode).value());

	// ****** Sound ****** //
	JsonObject rootSound = root.createNestedObject ("Sound");
	rootSound["On"]     = sound.isOn();
	rootSound["Volume"] = sound.getVolume().value();
	rootSound["Mode"]   = (uint8_t) sound.getMode();

	if (pretty)
		serializeJsonPretty (root, string);
	else
		serializeJson (root, string);
} // Json::generateData

String Json::getResourcesPretty ()
{
	String string;

	generateResources (string, true);

	return string;
}

String Json::getResources ()
{
	String string;

	generateResources (string, false);

	return string;
}

void Json::generateResources (String & string, bool pretty)
{
	const int colorNRows    = sizeof(WebcolorList) / sizeof(*WebcolorList);
	const int colorNColumns = sizeof(*WebcolorList) / sizeof(**WebcolorList);

	const size_t capacity = 0
	  + JSON_OBJECT_SIZE (3)                         // root (Status/Message/Light)
	  + JSON_OBJECT_SIZE (2)                         // light (ModeNames/Colors)
	  + JSON_ARRAY_SIZE (LightMode::N)               // modeNames array
	  + LightMode::N * 15                            // modeNames Strings
	  + (JSON_ARRAY_SIZE (colorNRows))               // color array
	  + colorNRows * JSON_ARRAY_SIZE (colorNColumns) // color[i] array
	;
	DynamicJsonDocument root (capacity);

	root["Status"]  = "OK";
	root["Message"] = "";

	// ****** Light ****** //
	JsonObject rootLight = root.createNestedObject ("Light");

	// -- ModeNames -- //
	JsonArray rootLightModeNames = rootLight.createNestedArray ("ModeNames");
	for (LightMode mode = LightMode::MIN; mode <= LightMode::MAX; mode++)
		rootLightModeNames.add (mode.toString());

	// -- Colors -- //
	JsonArray rootLightColors = rootLight.createNestedArray ("Colors");
	for (int i = 0; i < colorNRows; i++)
	{
		JsonArray rootLightColorsElement = rootLightColors.createNestedArray();

		for (int j = 0; j < colorNColumns; j++)
			rootLightColorsElement.add (WebcolorList[i][j]);
	}

	if (pretty)
		serializeJsonPretty (root, string);
	else
		serializeJson (root, string);
} // Json::generateResources

String Json::getErrorPretty (RequestError error)
{
	String string;

	generateError (string, true, error);

	return string;
}

String Json::getError (RequestError error)
{
	String string;

	generateError (string, false, error);

	return string;
}

void Json::generateError (String & string, bool pretty, RequestError error)
{
	const char status[]  = "ERROR";
	const String message = error.toString();

	const size_t capacity =
	  JSON_OBJECT_SIZE (2)                // root (status/message)
	  + sizeof(status) + message.length() // status + error (Strings)
	;

	DynamicJsonDocument root (capacity);

	root["Status"]  = status;
	root["Message"] = message;

	if (pretty)
		serializeJsonPretty (root, string);
	else
		serializeJson (root, string);
}

Json json = Json();

#endif // if defined(LUMOS_ESP8266)