// Liquid Crystal Display Library
#include <LiquidCrystal.h>

// Pins for LCD Screen
const int D4 = 5, D5 = 4, D6 = 3, D7 = 2, RS = 12, EN = 11;

// If the diode value is less than this, we are blocked
const int LIMVAL = 1;
// Number of inches between A and B
const long LENGTH = 11;
// Debug or not
const long configdebug = 0;

// Width of number to display
const int DISPLEN = 12;

// Speed Conversion Constants
const long MSPERS = 1000;
const long X1 = 100;
const long X2 = 11;
const float X3 = (float)(LENGTH*X1*MSPERS);

// Strings for display screen
char timeString[17];
char speedString[17];

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

// LCD Screen Initialization
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

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
  
  // Set up end of time string
  timeString[12] = ' ';
  timeString[13] = 'm';
  timeString[14] = 's';
  timeString[15] = ' ';
  timeString[16] = '\0';
  
  // Set up end of speed string
  speedString[12] = ' ';
  speedString[13] = 'm';
  speedString[14] = 'p';
  speedString[15] = 'h';
  speedString[16] = '\0';
  
  // Begin Measurement
  Serial.begin(9600);
  curtime = millis();
  Serial.print("Ready at ");
  Serial.print(curtime);
  Serial.print("\n");
  
  // Activate LCD Screen
  lcd.begin(16, 2);
  // Print Initialization Message
  lcd.setCursor(0, 0);
  lcd.print("RailSpeed");
  lcd.setCursor(0, 1);
  lcd.print("Initialized");
}

float milesPerHour(unsigned long timeElasped) {
  float mph = (float)(X2*timeElasped);
  mph = X3/mph;
  return mph;
}

void screenDisplay() {
  // Convert speed to unsigned long
  unsigned long speedTemp = (unsigned long)(speedVal+0.5);
  unsigned long timeTemp = timeT;
  int itr = DISPLEN-1;
  
  // Loop through time string
  while(itr >= 0) {
    if(timeTemp != 0) {
      timeString[itr] = (char)((timeTemp % 10) + 48);
    } else {
      timeString[itr] = ' ';
    }
    timeTemp = timeTemp/10;
    itr--;
  }
  
  // Loop through speed string
  itr = DISPLEN-1;
  while(itr >= 0) {
    if(speedTemp != 0) {
      speedString[itr] = (char)((speedTemp % 10) + 48);
    } else {
      speedString[itr] = ' ';
    }
    speedTemp = speedTemp/10;
    itr--;
  }
  
  // Display the time and speed
  lcd.setCursor(0, 0);
  lcd.print(timeString);
  lcd.setCursor(0, 1);
  lcd.print(speedString);
}

void loop() {
  // Read Current Time and Pin Values
  delay(25);
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
  
  
  // State Logic
  if(currentState == A_FWD) {
    // Value Resolution
    if(valA > LIMVAL) {
      // If A becomes unblocked, move to L_FWD
      Serial.print("A_FWD to L_FWD - A = ");
      Serial.print(valA);
      Serial.print("\n");
      nextState = L_FWD;
    }
    else {
      // Otherwise, stay in A_FWD
      nextState = A_FWD;
    }
  } 
  else if(currentState == L_FWD) {
    // Value Resolution
    if(valB < LIMVAL) {
      // If B becomes blocked, move to B_FWD
      Serial.print("L_FWD to B_FWD - B = ");
      Serial.print(valB);
      Serial.print("\n");
      nextState = B_FWD;
      timeB = curtime;
    }
    else {
      // Otherwise, stay in L_FWD
      nextState = L_FWD;
    }
  }
  else if(currentState == B_FWD) {
    // Value Resolution
    if(valB > LIMVAL) {
      // If B becomes unblocked, move to D_TIM and calculate
      Serial.print("B_FWD to D_TIM - B = ");
      Serial.print(valB);
      Serial.print("\n");
      nextState = D_TIM;
      timeT = timeB - timeA;
      speedVal = milesPerHour(timeT);
    }
    else {
      nextState = B_FWD;
    }
  }
  else if(currentState == B_REV) {
    // Value Resolution
    if(valB > LIMVAL) {
      // If B becomes unblocked, move to L_REV
      Serial.print("B_REV to L_REV - B = ");
      Serial.print(valB);
      Serial.print("\n");
      nextState = L_REV;
    }
    else {
      // Otherwise, stay in A_FWD
      nextState = B_REV;
    }
  }
  else if(currentState == L_REV) {
    // Value Resolution
    if(valA < LIMVAL) {
      // If A becomes blocked, move to A_REV
      Serial.print("L_REV to A_REV - A = ");
      Serial.print(valA);
      Serial.print("\n");
      nextState = A_REV;
      timeA = curtime;
    }
    else {
      // Otherwise, stay in L_REV
      nextState = L_REV;
    }
  }
  else if(currentState == A_REV) {
    // Value Resolution
    if(valA > LIMVAL) {
      // If A becomes unblocked, move to D_TIM and calculate
      Serial.print("A_REV to D_TIM - A = ");
      Serial.print(valA);
      Serial.print("\n");
      nextState = D_TIM;
      timeT = timeA - timeB;
      speedVal = milesPerHour(timeT);
    }
    else {
      nextState = A_REV;
    }
  }
  else if(currentState == D_INI) {
    // Value Resolution
    if(valA < LIMVAL) {
      // If A is blocked, move to A_FWD
      Serial.print("D_INI to A_FWD - A = ");
      Serial.print(valA);
      Serial.print("\n");
      nextState = A_FWD;
      timeA = curtime;
      timeB = 0;
      timeT = 0;
      speedVal = 0.0;
    }
    else if(valB < LIMVAL) {
      // If B is blocked, move to B_REV
      Serial.print("D_INI to B_REV - B = ");
      Serial.print(valB);
      Serial.print("\n");
      nextState = B_REV;
      timeA = 0;
      timeB = curtime;
      timeT = 0;
      speedVal = 0.0;
    }
    else {
      // Otherwise, remain in D_INI
      nextState = D_INI;
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
    screenDisplay();
    
    // Move to D_INI
    // Later on there may be a delay associated with this state
    Serial.print("Delaying for ");
    Serial.print(timeT);
    Serial.print(" milliseconds\n");
    delay(timeT);
    Serial.print("D_TIM to D_INI\n");
    nextState = D_INI;
  }
  else {
    // Display the Error
    Serial.println("State Machine Error");
    Serial.print("UNKNOWN to D_INI\n");
    
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
