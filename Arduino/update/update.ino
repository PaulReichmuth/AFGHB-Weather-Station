#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SenseBoxMCU.h"
#include <math.h>

#define OLED_RESET 4

String dir[8] = {"N", "NE", "E", "SE", "S", "SW", "W", "NW"};
int dirnum = 0;
int samplesize = 60;
int imageselector = 1;

HDC1080 hdc;
VEML6070 vml;
TSL45315 lux_sensor;
Adafruit_SSD1306 display(OLED_RESET);

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  senseBoxIO.statusRed();
  senseBoxIO.powerNone();
  int ms = 0;
  while (!Serial);
  if (Serial)
  {
    senseBoxIO.statusNone();
    senseBoxIO.statusGreen();
    delay(250);
    senseBoxIO.powerXB1(true);
    delay(250);
    senseBoxIO.powerUART(true);
    delay(250);
    senseBoxIO.powerI2C(true);
    delay(250);
    senseBoxIO.powerXB2(true);
    //Serial.print("Samplesize: ");
    //Serial.println(samplesize);
  }
  hdc.begin();
  vml.begin();
  lux_sensor.begin();
  delay(2000);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3D);
  display.display();
  delay(100);
  display.clearDisplay();
  display.display();
}

void loop()
{
  int speeds[60];
  int dirs[60];
  int temps[60];
  int humis[60];
  int uvs[60];
  int lights[60];
  for (int i; i <= samplesize; i++)
  {
    speeds[i] = random(80);
    dirs[i] = random(8);
    temps[i] = hdc.getTemperature();
    humis[i] = hdc.getHumidity();
    uvs[i] = vml.getUvIntensity();
    lights[i] = lux_sensor.getIlluminance();
    display.clearDisplay();
    display.println(String(Serial.read()));
    display.display();
    delay(1000);
  }
  int avgdir = round(average(dirs, 60));
  if (avgdir > 8)
  {
    avgdir = 8;
  }
  int avgtemp = round(average(temps, 60));
  int avghumi = round(average(humis, 60));
  int avguv = round(average(uvs, 60));
  int avglight = round(average(lights, 60));

  Serial.print("Wind ");
  Serial.print(avgdir);
  Serial.println(" ");
  Serial.print("Speed ");
  Serial.print(round(average(speeds, 60)));
  Serial.println(" ");
  Serial.print("Temp ");
  Serial.print(avgtemp);
  Serial.println(" ");
  Serial.print("Humi ");
  Serial.print(avghumi);
  Serial.println(" ");
  Serial.print("UV-raw ");
  Serial.print(avguv);
  Serial.println(" ");
  Serial.print("UV-Index ");
  Serial.print(getUVI(avguv));
  Serial.println(" ");
  Serial.print("Lux ");
  Serial.print(avglight);
  Serial.println(" ");
  Serial.println();

  if (!Serial)
  {
    display.clearDisplay();
    display.display();
    senseBoxIO.statusNone();
    senseBoxIO.statusRed();
    delay(250);
    senseBoxIO.powerXB1(false);
    delay(250);
    senseBoxIO.powerUART(false);
    delay(250);
    senseBoxIO.powerI2C(false);
    delay(250);
    senseBoxIO.powerXB2(false);
    setup();
  }
}

float average(int *array, int len) // assuming array is int.
{
  long sum = 0L; // sum will be larger than an item, long for safety.
  for (int i = 0; i < len; i++)
    sum += array[i];
  return ((float)sum) / len; // average will be fractional, so float may be appropriate.
}

float getUVI(int uv)
{ //vml into Uv-Index

  float refVal = 0.4; // Referenzwert: 0,01 W/m^2 ist äquivalent zu 0.4 als UV-Index
  float uvi = refVal * (uv * 5.625) / 1000;
  return uvi;
}
