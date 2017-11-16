// Diode Pins
const int pinA = A0;
const int pinB = A1;

// Delay time
const unsigned long deltime = 25;

// Current Time
unsigned long curtime;
// Next Time
unsigned long nextime;

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
  nextime = 0;
  Serial.print("Ready at ");
  Serial.print(curtime);
  Serial.print("\n");
}

void loop() {
  // Read Current Time and Pin Values
  curtime = millis();
  nextime = curtime + deltime;
  valA = analogRead(pinA);
  valB = analogRead(pinB);
  
  // Print Values
  Serial.print(curtime);
  Serial.print("\t");
  Serial.print(valA);
  Serial.print("\t");
  Serial.print(valB);
  Serial.print("\n");

  // Delay until next time
  while(nextime > millis()) {
    delay(1);
  }
}
