String dir[8] = {"N","NE","E","SE","S","SW","W","NW"};
int dirnum = 0;

void setup() {
Serial.begin(115200);
while(!Serial);
delay(1000);


}

void loop() {
Serial.print("Wind ");
Serial.println(dirnum);
Serial.print("Speed ");
Serial.println(random(analogRead(2)));
delay(2500);
dirnum ++;
if(dirnum > 8){
  dirnum = 0;
}
}
