// Diode Pins
const int pinA = A0;
const int pinB = A1;

// Current Time
unsigned long curtime;

// Diode Output Values
int valA;
int valB;

void setup() {
  // Initialize Values
  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);
  valA = 0;
  valB = 0;
  
  // Begin Measurement
  Serial.begin(9600);
  curtime = millis();
  Serial.print("Ready at ");
  Serial.print(curtime);
  Serial.print("\n");
}

float milesPerHour(unsigned long timeElasped) {
  float mph = (float)(X2*timeElasped);
  mph = X3/mph;
  return mph;
}

void loop() {
  // Read Current Time and Pin Values
  //delay(25);
  curtime = millis();
  valA = analogRead(pinA);
  valB = analogRead(pinB);
  
  if(configdebug == 1) {
    Serial.print("A = ");
    Serial.print(valA);
    Serial.print(" B = ");
    Serial.print(valB);
    Serial.print("\n");
  }
}
