#define LIMVAL 100

// Current Time
unsigned long curtime;

// Diode Pins
const int pinA = A0;
const int pinB = A1;

// Diode Output Values
int valA;
int valB;

// Time Values
unsigned long timeA;
unsigned long timeB;
unsigned long timeT;

// Speed Value
float speedVal;

// States
enum speedStates {
  A_FWD, // A forward
  B_FWD, // B forward
  A_REV, // A reverse
  B_REV, // B reverse
  D_INI, // Display initialize
  D_TIM  // Display time
};
enum speedStates currentState;
enum speedStates nextState;

void setup() {
  // Initialize Values
  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);
  valA = 0;
  valB = 0;
  currentState = D_INI;
  nextState = D_INI;
  timeA = 0;
  timeB = 0;
  timeT = 0;
  speedVal = 0.0;
  
  // Begin Measurement
  Serial.begin(9600);
  curtime = millis();
  Serial.print("Ready ");
  Serial.print(curtime);
  Serial.print("\n");
}

void loop() {
  // Read Current Time
  curtime = millis();
  
  // State Logic
  if(currentState == A_FWD) {
    //
  } 
  else if(currentState == B_FWD) {
    //
  }
  else if(currentState == A_REV) {
    //
  }
  else if(currentState == B_REV) {
    //
  }
  else if(currentState == D_INI) {
    //
  }
  else if(currentState == D_TIM) {
    //
  }
  else {
    Serial.println("State Machine Error");
    nextState = D_INI;
  }
  
  // Set Current State to Next State
  currentState = nextState;
}
