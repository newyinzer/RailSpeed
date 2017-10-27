// If the diode value is less than this, we are blocked
const int LIMVAL = 100;
// Number of inches between A and B
const int LENGTH = 11;

// Speed Conversion Constants
const int X1 = 100;
const int X2 = 11;
const float X3 = (float)(LENGTH*X1);

// Diode Pins
const int pinA = A0;
const int pinB = A1;

// Current Time
unsigned long curtime;

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
  L_FWD, // A to B forward
  B_FWD, // B forward
  A_REV, // A reverse
  L_REV, // B to A reverse
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

float milesPerHour(int timeElasped) {
  float mph = (float)(X2*timeElasped);
  mph = X3/mph;
  return mph;
}

void loop() {
  // Read Current Time and Pin Values
  curtime = millis();
  valA = analogRead(pinA);
  valB = analogRead(pinB);
  
  // State Logic
  if(currentState == A_FWD) {
    // Value Resolution
    if(valA > LIMVAL) {
      // If A becomes unblocked, move to L_FWD
      nextState = L_FWD;
    }
    else {
      // Otherwise, stay in A_FWD
      nextState = A_FWD;
    }
    timeA = timeA;
    timeB = timeB;
    timeT = timeT;
    speedVal = speedVal;
  } 
  else if(currentState == L_FWD) {
    // Value Resolution
    if(valB < LIMVAL) {
      // If B becomes blocked, move to B_FWD
      nextState = B_FWD;
      timeB = curtime;
    }
    else {
      // Otherwise, stay in L_FWD
      nextState = L_FWD;
      timeB = timeB;
    }
    timeA = timeA;
    timeT = timeT;
    speedVal = speedVal;
  }
  else if(currentState == B_FWD) {
    // Value Resolution
    if(valB > LIMVAL) {
      // If B becomes unblocked, move to D_TIM and calculate
      nextState = D_TIM;
      timeT = timeB - timeA;
      speedVal = milesPerHour(timeT);
    }
    else {
      nextState = B_FWD;
      timeT = timeT;
      speedVal = speedVal;
    }
    timeA = timeA;
    timeB = timeB;
  }
  else if(currentState == B_REV) {
    //
  }
  else if(currentState == L_REV) {
    //
  }
  else if(currentState == A_REV) {
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
