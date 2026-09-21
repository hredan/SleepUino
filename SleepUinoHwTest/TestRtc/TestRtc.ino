// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include "RTClib.h"

// I2C SLC D6, SDA D7 Used for Real Time Clock and Display
const int SLC_PIN = 12;
const int SDA_PIN = 13;

RTC_DS3231 rtc;

void setup () {
  Serial.begin(115200);
  // I2C Bus used for real time clock and display
  Wire.begin(SDA_PIN, SLC_PIN);
  // Without the delay real time clock can not initilized successfully, seems to
  // be that the I2C initializing needs a bit time (300 ms)
  delay(300);

#ifndef ESP8266
  while (!Serial); // wait for serial port to connect. Needed for native USB
#endif

  while (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    delay(300);
  }

  rtc.adjust(DateTime(2026, 1, 1, 5, 57, 0));

}

void loop () {
    DateTime now = rtc.now();

    char buffer[20];
    sprintf(buffer, "%d:%d:%d", now.hour(), now.minute(), now.second());
    Serial.println(buffer);  //one line of code

    Serial.print("Temperature: ");
    Serial.print(rtc.getTemperature());
    Serial.println(" C");

    Serial.println();
    delay(3000);
}
