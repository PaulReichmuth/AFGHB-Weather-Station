void setup() {
Serial.begin(115200);
while(!Serial);
delay(1000);


}

void loop() {
Serial.println("Wind NNE");
Serial.print("Speed ");
Serial.println(random(analogRead(2)));
delay(2500);

}
