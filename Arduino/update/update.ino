#include <Wire.h>
#include "SenseBoxMCU.h"
#include <math.h>
String dir[8] = {"N", "NE", "E", "SE", "S", "SW", "W", "NW"};
int dirnum = 0;
int samplesize = 60;

HDC1080 hdc;
VEML6070 vml;
BMP280 bmp;
TSL45315 lux;


void setup() {
  Serial.begin(115200);
  Wire.begin();
  senseBoxIO.statusRed();
  senseBoxIO.powerNone();
  while (!Serial);
  if (Serial) {
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
  bmp.begin();
  lux.begin();
}

void loop() {
  float speeds[60];
  float dirs[60];
  float temps[60];
  float humis[60];
  float uvs[60];
  float lights[60];
  float pressures[60];
  float bmptemps[60];
  for (int i; i <= samplesize; i ++) {
    speeds[i] = random(80);
    dirs[i] = random(16);
    temps[i] = hdc.getTemperature();
    humis[i] = hdc.getHumidity();
    uvs[i] = vml.getUvIntensity();
    pressures[i] = bmp.getPressure();
    bmptemps[i] = bmp.getTemperature();
    lights[i] = lux.getIlluminance();
    Serial.print("Took Sample Num.: ");
    Serial.println(i);
    Serial.println(pressures[i]);
    delay(1000);
  }
  int avgdir = round(average(dirs, 60));
  if (avgdir > 16){
    avgdir = 16;
  }
  float avgtemp = average(temps, 60);
  float avghumi = average(humis, 60);
  float avguv = average(uvs, 60);
  float avglight = average(lights, 60);
  float avgpress = average(pressures, 60) /100;
  float avgreftemp = average(bmptemps, 60);
  
  Serial.println("---BEGIN---");
  Serial.print("Wind ");
  Serial.println(avgdir);
  Serial.print("Speed ");
  Serial.println(round(average(speeds, 60)));
  Serial.print("Temp ");
  Serial.println(avgtemp);
  Serial.print("Humi ");
  Serial.println(avghumi);
  Serial.print("UV-raw ");
  Serial.println(avguv);
  Serial.print("UV-Index " );
  Serial.println(getUVI(avguv));
  Serial.print("Lux " );
  Serial.println(avglight);
  Serial.print("Presssure ");
  Serial.println(avgpress);
  Serial.println("---END---");
  if (!Serial) {
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

float average (float * array, int len)  // assuming array is int.
{
  long sum = 0L ;  // sum will be larger than an item, long for safety.
  for (int i = 0 ; i < len ; i++)
    sum += array [i] ;
  return  ((float) sum) / len ;  // average will be fractional, so float may be appropriate.
}

float getUVI(int uv) {//vml into Uv-Index

  float refVal = 0.4; // Referenzwert: 0,01 W/m^2 ist äquivalent zu 0.4 als UV-Index
  float uvi = refVal * (uv * 5.625) / 1000;
  return uvi;
}
