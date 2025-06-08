#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"

// OLED display configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Air quality sensor buffer configuration
#define LENG 31   // Buffer length for PMS5003 sensor (31 bytes + 0x42 = 32 bytes)
unsigned char buf[LENG];

// DHT22 configuration
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Global variables for PM values
int PM01Value = 0;  // PM1.0 value (ug/m3)
int PM2_5Value = 0; // PM2.5 value (ug/m3)
int PM10Value = 0; // PM10 value (ug/m3)

void setup() {
  Serial.begin(9600);
  Serial.println("Air Quality and DHT22 Sensor Test");

  // Initialize DHT22 sensor
  dht.begin();
  delay(2000); // Wait for sensor stabilization

  // Initialize OLED display
  if (!OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED initialization failed! Check connections."));
    while (1); // Halt if OLED fails
  }
  OLED.clearDisplay();
  OLED.setTextColor(WHITE, BLACK);
  OLED.setTextSize(1);
  OLED.setCursor(0, 0);
  OLED.println("Initializing...");
  OLED.display();
}

void loop() {
  // Read air quality sensor data
  if (Serial.find(0x42, 1000)) { // Wait for start byte 0x42 with 1-second timeout
    if (Serial.readBytes(buf, LENG) == LENG) { // Ensure complete data frame
      if (buf[0] == 0x4d && checkValue(buf, LENG)) { // Verify frame start and checksum
        PM01Value = transmitPM01(buf);   // Read PM1.0
        PM2_5Value = transmitPM2_5(buf); // Read PM2.5
        PM10Value = transmitPM10(buf);   // Read PM10
      } else {
        Serial.println("Invalid frame or checksum error!");
      }
    } else {
      Serial.println("Incomplete data received!");
    }
  } else {
    Serial.println("No start byte found!");
  }

  // Update display every 1 second
  static unsigned long OledTimer = millis();
  if (millis() - OledTimer >= 1000) {
    OledTimer = millis();

    // Print to Serial monitor
    Serial.print("PM1.0: ");
    Serial.print(PM01Value);
    Serial.println(" ug/m3");
    Serial.print("PM2.5: ");
    Serial.print(PM2_5Value);
    Serial.println(" ug/m3");
    Serial.print("PM10: ");
    Serial.print(PM10Value);
    Serial.println(" ug/m3");
    Serial.println();

    // Update OLED display
    OLED.clearDisplay();
    OLED.setTextColor(WHITE, BLACK);
    OLED.setTextSize(1);
    OLED.setCursor(0, 0);
    OLED.println("  Air Quality Sensor");

    OLED.print("   PM1.0 = ");
    OLED.print(PM01Value);
    OLED.println(" ug/m3");
    OLED.print("   PM2.5 = ");
    OLED.print(PM2_5Value);
    OLED.println(" ug/m3");
    OLED.print("   PM10  = ");
    OLED.print(PM10Value);
    OLED.println(" ug/m3");

    // Read DHT22 sensor
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    if (isnan(t) || isnan(h)) {
      OLED.println("DHT22 read failed!");
      Serial.println("DHT22 read failed!");
    } else {
      OLED.print(" Temp = ");
      OLED.print(t); // Removed -5 offset; assumed calibration not needed
      OLED.print(" C Hum = ");
      OLED.print(h);
      OLED.println("%");
    }
    OLED.display();
  }
}

// Checksum verification for air quality sensor data
char checkValue(unsigned char *thebuf, char leng) {
  char receiveflag = 0;
  int receiveSum = 0;

  for (int i = 0; i < (leng - 2); i++) {
    receiveSum += thebuf[i];
  }
  receiveSum += 0x42; // Add start byte to checksum (per PMS5003 protocol)

  if (receiveSum == ((thebuf[leng - 2] << 8) + thebuf[leng - 1])) {
    receiveSum = 0;
    receiveflag = 1;
  }
  return receiveflag;
}

// Extract PM1.0 value from buffer
int transmitPM01(unsigned char *thebuf) {
  int PM01Val;
  PM01Val = ((thebuf[3] << 8) + thebuf[4]); // PM1.0 in ug/m3
  return PM01Val;
}

// Extract PM2.5 value from buffer
int transmitPM2_5(unsigned char *thebuf) {
  int PM2_5Val;
  PM2_5Val = ((thebuf[5] << 8) + thebuf[6]); // PM2.5 in ug/m3
  return PM2_5Val;
}

// Extract PM10 value from buffer
int transmitPM10(unsigned char *thebuf) {
  int PM10Val;
  PM10Val = ((thebuf[7] << 8) + thebuf[8]); // PM10 in ug/m3
  return PM10Val;
}
