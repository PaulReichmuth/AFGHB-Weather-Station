#include <Wire.h>
#include <senseBoxIO.h>
#include <Adafruit_HDC1000.h>
#include <math.h>
String dir[8] = {"N", "NE", "E", "SE", "S", "SW", "W", "NW"};
int dirnum = 0;
int samplesize = 60;

Adafruit_HDC1000 hdc;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  senseBoxIO.statusRed();
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
  hdc.begin(0x43);
  
}

void loop() {
  int speeds[60];
  int dirs[60];
  int temps[60];
  int humis[60];
  for (int i; i <= samplesize; i ++) {
    speeds[i] = random(analogRead(6));
    dirs[i] = random(8);
    temps[i] = hdc.readTemperature();
    humis[i] = hdc.readHumidity();
    Serial.print("Took Sample Num.: ");
    Serial.println(i);
    delay(1000);
  }
  int avgdir = round(average(dirs, 60));
  if (avgdir > 8){
    avgdir = 8;
  }
  int avgtemp = round(average(temps, 60));
  int avghumi = round(average(humis, 60));
  
  Serial.print("Wind ");
  Serial.println(avgdir);
  Serial.print("Speed ");
  Serial.println(round(average(speeds, 60)));
  Serial.print("Temp ");
  Serial.println(avgtemp);
  Serial.print("Humi ");
  Serial.println(avghumi);
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

float average (int * array, int len)  // assuming array is int.
{
  long sum = 0L ;  // sum will be larger than an item, long for safety.
  for (int i = 0 ; i < len ; i++)
    sum += array [i] ;
  return  ((float) sum) / len ;  // average will be fractional, so float may be appropriate.
}
