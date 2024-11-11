/*
  This software is based on code from Adafruit. The original code is licensed under the MIT License, 
  and the copyright belongs to Adafruit Industries. This modified version includes substantial changes.

  Original Copyright (c) 2020 Adafruit Industries
  Additional modifications Copyright (c) 2024 George DiNicola

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and 
  associated documentation files (the "Software"), to deal in the Software without restriction, including 
  without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
  copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the 
  following conditions:

  [Include the original license text here, or refer to it if included in another file]
*/

#include "Adafruit_seesaw.h"
#include "RTClib.h"

Adafruit_seesaw ss;
RTC_DS1307 rtc;  // Declare rtc as an instance of RTC_DS1307

const unsigned long BAUD_RATE = 115200;
const unsigned long WAIT_TIME_MS = 60000;  // 1 minute

void setup() {
  Serial.begin(BAUD_RATE);

  // Initialize Adafruit STEMMA Soil Sensor
  if (!ss.begin(0x36)) {
    Serial.println("Error: Adafruit soil sensor not found. Delaying program to avoid excessive CPU usage");
    while (true) {
      delay(WAIT_TIME_MS);
    }
  }

  // Initialize RTC module
  if (!rtc.begin()) {
    Serial.println("Error: RTC module not found. Delaying program to avoid excessive CPU usage");
    while (true) {
      delay(WAIT_TIME_MS);
    }
  }

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));  // Set RTC to the compile date and time
}

void loop() {
  float tempC = ss.getTemp();
  uint16_t capread = ss.touchRead(0);

  // Output the Current Datetime
  DateTime now = rtc.now();
  char currentDateTime[50];
  sprintf(currentDateTime, "Date and Time (PST): %d/%02d/%02d %02d:%02d:%02d",
    now.year(),
    now.month(),
    now.day(),
    now.hour(),
    now.minute(),
    now.second()
  );
  Serial.println(currentDateTime);

  Serial.print("Temperature: "); Serial.print(tempC); Serial.println("*C");
  Serial.print("Capacitive: "); Serial.println(capread);
  delay(WAIT_TIME_MS);
}
