#include <senseBoxIO.h>
String dir[8] = {"N", "NE", "E", "SE", "S", "SW", "W", "NW"};
int dirnum = 0;

void setup() {
  Serial.begin(115200);
  senseBoxIO.statusRed();
  while (!Serial);
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
}

void loop() {
  Serial.print("Wind ");
  Serial.println(dirnum);
  Serial.print("Speed ");
  Serial.println(random(analogRead(2)));
  delay(2500);
  dirnum ++;
  if (dirnum > 8) {
    dirnum = 0;
  }
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
