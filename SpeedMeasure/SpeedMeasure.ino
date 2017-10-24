// If the diode value is less than this, we are blocked
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
  // Read Current Time and Pin Values
  curtime = millis();
  valA = analogRead(pinA);
  valB = analogRead(pinB);
  
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
    // Value Resolution
    if(valA < LIMVAL) {
      // If A is blocked, move to A_FWD
      nextState = A_FWD;
      timeA = curtime;
      timeB = 0;
      timeT = 0;
      speedVal = 0.0;
    }
    else if(valB < LIMVAL) {
      // If B is blocked, move to B_REV
      nextState = B_REV;
      timeA = 0;
      timeB = curtime;
      timeT = 0;
      speedVal = 0.0;
    }
    else {
      // Otherwise, remain in D_INI
      nextState = D_INI;
      timeA = 0;
      timeB = 0;
      timeT = 0;
      speedVal = 0.0;
    }
  }
  else if(currentState == D_TIM) {
    // Display the time and speed
    Serial.print("Time Elapsed: ");
    Serial.print(timeT);
    Serial.print(" milliseconds\n");
    Serial.print("Speed: ");
    Serial.print(speedVal);
    Serial.print(" Miles per Hour\n");
    
    // Value Resolution
    nextState = D_INI;
    timeA = timeA;
    timeB = timeB;
    timeT = timeT;
    speedVal = speedVal;
  }
  else {
    // Display the Error
    Serial.println("State Machine Error");
    
    // Value Resolution
    nextState = D_INI;
    timeA = 0;
    timeB = 0;
    timeT = 0;
    speedVal = 0.0;
  }
  
  // Set Current State to Next State
  currentState = nextState;
}
