/* Example implementation of an alarm using DS3231
 * Based on the example DS3231_alarm at
 * https://github.com/adafruit/RTClib/tree/master/examples/DS3231_alarm
 *
 * VCC and GND of RTC should be connected to some power source
 * SDA, SCL of RTC should be connected to SDA, SCL of arduino
 * SQW should be connected to CLOCK_INTERRUPT_PIN
 * CLOCK_INTERRUPT_PIN needs to work with interrupts
 */

#include <RTClib.h>
// #include <Wire.h>

RTC_DS3231 rtc;

// the pin that is connected to SQW
#define CLOCK_INTERRUPT_PIN 2

static bool firedStatus = false;

void setup() {
    Serial.begin(9600);

    // initializing the rtc
    if(!rtc.begin()) {
        Serial.println("Couldn't find RTC!");
        Serial.flush();
        while (1) delay(10);
    }

    if(rtc.lostPower()) {
        // this will adjust to the date and time at compilation
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    //we don't need the 32K Pin, so disable it
    rtc.disable32K();

    // Making it so, that the alarm will trigger an interrupt
    pinMode(CLOCK_INTERRUPT_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(CLOCK_INTERRUPT_PIN), onAlarm, FALLING);

    // set alarm 1, 2 flag to false (so alarm 1, 2 didn't happen so far)
    // if not done, this easily leads to problems, as both register aren't reset on reboot/recompile
    rtc.clearAlarm(1);
    rtc.clearAlarm(2);

    // stop oscillating signals at SQW Pin
    // otherwise setAlarm1 will fail
    rtc.writeSqwPinMode(DS3231_OFF);

    // turn off alarm 2 (in case it isn't off already)
    // again, this isn't done at reboot, so a previously set alarm could easily go overlooked
    rtc.disableAlarm(2);

    // schedule an alarm
    if(!rtc.setAlarm1(
            // Time relative to now
            //rtc.now() + TimeSpan(10), DS3231_A1_Second // this mode triggers the alarm when the seconds match. See Doxygen for other options
            rtc.now() + TimeSpan(0, 0, 1, 0), DS3231_A1_Minute  // In 1 minutes time from now
            //rtc.now() + TimeSpan(0, 0, 2, 10), DS3231_A1_Minute // In 2 minutes and 10 seconds time from now
            //rtc.now() + TimeSpan(0, 1, 1, 0), DS3231_A1_Hour    // In 1 hour and 1 minutes time from now

            // At explicit times
            //DateTime(0, 0, 0, 0, 0, 0), DS3231_A1_Second  // When seconds at zero, that is every minute
            //DateTime(0, 0, 0, 0, 0, 0), DS3231_A1_Minute  // When the minutes and seconds are at zero, that is, every hour
            //DateTime(0, 0, 0, 0, 1, 10), DS3231_A1_Minute // When the minutes are at 1 and seconds are at 10, that is 1 minute and 10 seconds past the hour
            //DateTime(0, 0, 0, 10, 0, 0), DS3231_A1_Hour   // At 10:00 o'clock every day
            //DateTime(0, 0, 0, 10, 18, 0), DS3231_A1_Hour  // At 10:18 every day
    )) {
        Serial.println("Error, alarm wasn't set!");
    }else {
        Serial.print("Current time: ");
        char date[10] = "hh:mm:ss";
        rtc.now().toString(date);
        Serial.println(date);
    }
}

void loop() {

    if (firedStatus == true) {
        // Print time alarm occured
        Serial.print("Alarm occured! ");
        char date[10] = "hh:mm:ss";
        rtc.now().toString(date);
        Serial.println(date);

        // Reset alarm status flag
        firedStatus = false;

        // Clear alarm
        rtc.clearAlarm(1);
        Serial.println("Alarm cleared");
    }
}

void onAlarm() {
  // Set status flag
  firedStatus = true;
}
