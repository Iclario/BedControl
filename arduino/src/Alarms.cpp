#include "Alarms.h"
#include "ArduinoSerial.h"
#include "Logger.h"

Alarms::Alarms()
{ }

void Alarms::initAll ()
{
	initTimeSync();
	initDawn();
}

void Alarms::initTimeSync ()
{
	clearTimeSync();

	timeSyncTimer = Alarm.timerRepeat (0, 60, 0, ArduinoSerial::askForTime);
}

void Alarms::initDawn ()
{
	if (!MORNING_ALARM_ENABLED || timeStatus() == timeNotSet)
		return;

	Log.trace ("Setting morning alarm" dendl);

	clearDawn();

	// Start the alarm before so it finishes at the time requested
	morningAlarm = Alarm.alarmRepeat (WAKEUP_HOURS * 3600 + WAKEUP_MINUTES * 60 + WAKEUP_SECONDS - global.speed[MOD_DAWN] * 60, this->dawnStart);
}

void Alarms::clearAll ()
{
	clearTimeSync();
	clearDawn();
}

void Alarms::clearTimeSync ()
{
	Alarm.free (timeSyncTimer);
}

void Alarms::clearDawn ()
{
	Alarm.free (morningAlarm);
}

void Alarms::dawnStart ()
{
	global.mod = MOD_DAWN;
	global.on  = true;

	Log.info ("Starting dawn alert" dendl);
}

Alarms alarms = Alarms();