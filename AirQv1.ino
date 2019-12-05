#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"
Adafruit_SSD1306 OLED(-1);

#define LENG 31   //0x42 + 31 bytes equal to 32 bytes
#define DHTPIN 2
#define DHTTYPE DHT22
unsigned char buf[LENG];

int PM01Value = 0;        //define PM1.0 value of the air detector module
int PM2_5Value = 0;       //define PM2.5 value of the air detector module
int PM10Value = 0;       //define PM10 value of the air detector module

DHT dht(DHTPIN, DHTTYPE);
void setup()
{
  Serial.begin(9600);
  Serial.println("DHT22 test!");
  dht.begin();
  delay(2000);
  OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

void loop()
{
  if (Serial.find(0x42)) {  //start to read when detect 0x42
    Serial.readBytes(buf, LENG);

    if (buf[0] == 0x4d) {
      if (checkValue(buf, LENG)) {
        //PM01Value=transmitPM01(buf); //count PM1.0 value of the air detector module
        PM2_5Value = transmitPM2_5(buf); //count PM2.5 value of the air detector module
        PM10Value = transmitPM10(buf); //count PM10 value of the air detector module
      }
    }
  }

  static unsigned long OledTimer = millis();
  if (millis() - OledTimer >= 1000)
  {
    OledTimer = millis();

    Serial.print("PM1.0: ");
    Serial.print(PM01Value);
    Serial.println("  ug/m3");

    Serial.print("PM2.5: ");
    Serial.print(PM2_5Value);
    Serial.println("  ug/m3");

    Serial.print("PM1 0: ");
    Serial.print(PM10Value);
    Serial.println("  ug/m3");
    Serial.println();



    OLED.setTextColor(WHITE, BLACK);  //ตัวอักษรสีขาว ,ฉากหลังดำ
    OLED.clearDisplay(); // ลบหน้าจอ
    OLED.setTextSize(1); // ขนาดตัวอักษร
    OLED.setCursor(0, 0);
    OLED.println("  Air Quality Sensor");
    OLED.setTextSize(1); // ขนาดตัวอักษร


    OLED.print("   PM2.5 = ");
    OLED.print(PM2_5Value);
    OLED.println(" ug/m3");

    OLED.print("   PM10 = ");
    OLED.print(PM10Value);
    OLED.println(" ug/m3");

    int t = dht.readTemperature();
    int h = dht.readHumidity();
    OLED.print(" Temp = ");
    OLED.print(t - 5);
    OLED.print("c Hud = ");
    OLED.print(h);
    OLED.println("%");

    OLED.display(); // แสดงตัวอักษรทั้งหมด

  }
}

char checkValue(unsigned char *thebuf, char leng)
{
  char receiveflag = 0;
  int receiveSum = 0;

  for (int i = 0; i < (leng - 2); i++) {
    receiveSum = receiveSum + thebuf[i];
  }
  receiveSum = receiveSum + 0x42;

  if (receiveSum == ((thebuf[leng - 2] << 8) + thebuf[leng - 1])) //check the serial data
  {
    receiveSum = 0;
    receiveflag = 1;
  }
  return receiveflag;
}

//transmit PM Value to PC
int transmitPM2_5(unsigned char *thebuf)
{
  int PM2_5Val;
  PM2_5Val = ((thebuf[5] << 8) + thebuf[6]); //count PM2.5 value of the air detector module
  return PM2_5Val;
}

//transmit PM Value to PC
int transmitPM10(unsigned char *thebuf)
{
  int PM10Val;
  PM10Val = ((thebuf[7] << 8) + thebuf[8]); //count PM10 value of the air detector module
  return PM10Val;
}
