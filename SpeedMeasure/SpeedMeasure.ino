unsigned long curtime;
int pinA = A0;
int pinB = A1;

void setup() {

  Serial.begin(19200);
  Serial.println("Ready");
  curtime = millis();
  
}

void loop() {
  
  curtime = millis();
  
  //
}
