# Using the DS3231 alarm with the Adafruit RTClib library

The DS3231 Real Time Clock module includes two alarms. Support for these was added to the Adafruit RTClib. The examples here

 - Setting an alarm and polling the SQW pin to monitor its status (DS3231-RTClib-Adafruit-Alarm-Poll-SQW.ino)
 - Setting an alarm and polling using alarmFired (DS3231-RTClib-Adafruit-Alarm-Poll-alarmFired.ino)
 - Sending the Arduino to sleep and waking it using the alarm and SQW pin (DS3231-_RTClib-Adafruit-Alarm-Sleep.ino)

More details are available on my blog https://garrysblog.com

## Setting the alarm
**setAlarm1(DateTime, alarm_mode)**
**setAlarm2(DateTime, alarm_mode)**

## Alarm modes  
### For alarm 1
 - DS3231_A1_PerSecond     Once per second
 - DS3231_A1_Second        When seconds match
 - DS3231_A1_Minute        When minutes and seconds match
 - DS3231_A1_Hour          When hours, minutes, and seconds match
 - DS3231_A1_Date          When date, hours, minutes, and seconds match (day of month)
 - DS3231_A1_Day           When day, hours, minutes, and seconds match (day of week)

### For alarm 2
 - DS3231_A2_PerMinute     Once per minute (00 seconds of every minute)
 - DS3231_A2_Minute        When minutes and seconds match
 - DS3231_A2_Hour          When hours, minutes, and seconds match
 - DS3231_A2_Date          When date, hours, minutes, and seconds match
 - DS3231_A2_Day           When day, hours, minutes, and seconds match

## Examples
### Set to an explicit time
In this example the alarm will trigger when seconds, minutes and hours match, resulting in triggering once per day, every day at (15:34:00). The date, month and year are all ignored.

**setAlarm1(DateTime(2020, 6, 25, 15, 34, 0), DS3231_A1_Hour)**

### Using TimeSpan or unixtime to set to an amount from the current time
The current time needs to be called before setting the remaining examples as it is needed to calculate the end time. These examples set the alarm to 10 seconds from the current time.

**setAlarm1(now + TimeSpan(0, 0, 0, 10), DS3231_A1_Second)**
**setAlarm1(now.unixtime() + 10, DS3231_A1_Second)**
