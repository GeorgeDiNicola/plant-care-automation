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
RTC_DS1307 rtc;

const unsigned long CAP_THRESHOLD = 1000;
const int RELAY_PIN = 13;

const unsigned long ONE_MINUTE = 60000;
const unsigned long ONE_HOUR = 60000 * 60;

const unsigned long BAUD_RATE = 115200;
const unsigned long WAIT_TIME_MS = ONE_MINUTE;
const unsigned long PUMP_MIN_TIME_MS = 1000;
const unsigned long PUMP_MAX_TIME_MS = 30000;


void logError(const char* message) {
  Serial.print(F("Error: "));
  Serial.print(message);
}

bool runPumpFor(int seconds) {
  unsigned long milliseconds = (unsigned long)(seconds * 1000.0);

  if (milliseconds < PUMP_MIN_TIME_MS || milliseconds > PUMP_MAX_TIME_MS) {
    logError("Pump time needs to be set between 1 and 30 seconds");
    return false;
  }

  digitalWrite(RELAY_PIN, HIGH);
  delay(milliseconds);  
  digitalWrite(RELAY_PIN, LOW);

  return true;
}

void stopLoop() {
  while (true) {
    delay(WAIT_TIME_MS * 60);
  }
}

void setup() {
  Serial.begin(BAUD_RATE);

  // Initialize Adafruit STEMMA Soil Sensor
  if (!ss.begin(0x36)) {
    logError("Adafruit soil sensor not found. Delaying program to avoid excessive CPU usage");
    stopLoop();
  }

  // Initialize RTC module
  if (!rtc.begin()) {
    logError("RTC module not found. Delaying program to avoid excessive CPU usage");
    stopLoop();
  }

  // Initialize the 5V pump
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));  // Set RTC to the compile date and time
}

void loop() {
  float tempC = ss.getTemp();
  uint16_t capread = ss.touchRead(0);

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
 
  if (capread < CAP_THRESHOLD) {
    if (!runPumpFor(30)) {
      stopLoop();
    }
  }

  delay(WAIT_TIME_MS);
}
