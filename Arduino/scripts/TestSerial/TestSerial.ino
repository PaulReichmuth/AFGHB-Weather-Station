String dir[8] = {"N","NE","E","SE","S","SW","W","NW"};

void setup() {
Serial.begin(115200);
while(!Serial);
delay(1000);


}

void loop() {
Serial.print("Wind ");
Serial.println(dir[random(8)]);
Serial.print("Speed ");
Serial.println(random(analogRead(2)));
delay(2500);

}
