/*
  ===========================================================================
        Using the alarm on a DS3231 with the Adafruit RTClib library
  ===========================================================================
  
  Set an alarm on the DS3231 RTC module and monitor its status using the
  alarmFired funtion. This does not require monitoring the SQW pin and requies
  polling and may not be the best method of using the alarm.

  Example sets alarm to trigger in 10 seconds after boot and then
  resets to a further 10 seconds after each trigger.

  Tested on a Arduino Nano. 3 July 2020.
  
  This example: https://github.com/garrysblog https://garrysblog.com/
  Library: https://github.com/adafruit/RTClib
  RTClib command info: https://adafruit.github.io/RTClib/html/class_r_t_c___d_s3231.html
  
  Connections
  -----------
  SDA -> Arduino Analog (SDA pin)
  SCL -> Arduino Analog (SCL pin)
  VCC -> Arduino 5V
  GND -> Arduino GND
*/

#include "RTClib.h"
RTC_DS3231 rtc;

void setup () {
  Serial.begin(9600); // Start serial port for monitoring
  
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  // If required set time
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // To compiled time
  //rtc.adjust(DateTime(2020, 7, 3, 20, 0, 0)); // Or explicitly, e.g. July 3, 2020 at 8pm

  // Disable and clear both alarms
  rtc.disableAlarm(1);
  rtc.disableAlarm(2);
  rtc.clearAlarm(1);
  rtc.clearAlarm(2);
         
  DateTime now = rtc.now(); // Get current time

  // Print current time and date
  char buff[] = "Start time is hh:mm:ss DDD, DD MMM YYYY";
  Serial.println(now.toString(buff));

  // Set alarm time
  rtc.setAlarm1(now + TimeSpan(0, 0, 0, 10), DS3231_A1_Second); // In 10 seconds time
  //rtc.setAlarm1(DateTime(2020, 6, 25, 15, 0, 0), DS3231_A1_Hour); // Or can be set explicity
}

void loop () {

  // Check if alarm by polling by using alarmFired
  if (rtc.alarmFired(1) == true){
    
    // Print current time and date
    DateTime now = rtc.now(); // Get the current time
    char buff[] = "Alarm triggered at hh:mm:ss DDD, DD MMM YYYY";
    Serial.println(now.toString(buff));

    // Disable and clear alarm
    //rtc.disableAlarm(1); // Not used as only disables the alarm on the SQW pin 
    rtc.clearAlarm(1); 

    // Perhaps reset to new time if required
    rtc.setAlarm1(now + TimeSpan(0, 0, 0, 10), DS3231_A1_Second); // Set for another 10 seconds
  }
}
