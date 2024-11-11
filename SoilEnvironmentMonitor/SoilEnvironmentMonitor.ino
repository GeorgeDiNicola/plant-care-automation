#include "Adafruit_seesaw.h"
#include "RTClib.h"

Adafruit_seesaw ss;
RTC_DS1307 rtc;  // Declare rtc as an instance of RTC_DS1307

const unsigned long BAUD_RATE = 115200;
const unsigned long WAIT_TIME_MS = 60000;

void setup() {
  Serial.begin(BAUD_RATE);

  // Initialize Adafruit STEMMA Soil Sensor
  if (!ss.begin(0x36)) {
    Serial.println("Error: Adafruit soil sensor not found");
    while(1) delay(1);
  }

  // Initialize RTC module
  if (!rtc.begin()) {
    Serial.println("Error: RTC module not found. Stopping program");
    while (true) {
      delay(60000);  // Delay to avoid excessive CPU usage
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
