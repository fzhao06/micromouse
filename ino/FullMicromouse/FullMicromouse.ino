#include <math.h>

// Driving Vars/Funcs
// directions will be a character 'l' or not to represent left and right
const byte leftAnlgPin = 3 ;
const byte leftDigPin1 = 1 ;
const byte leftDigPin2 = 2 ;
const byte rightAnlgPin = 4 ;
const byte rightDigPin1 = 5 ;
const byte rightDigPin2 = 6 ;
const short turn90 = 150 ;
const short turn180 = 300 ;
byte leftSpeed = 15; // may not need
byte rightSpeed = 20 ; // may not need

void driveForward(int pwmVal1, int pwmVal2) ;
void adjSpeed(char dir, byte spd) ; // will adjust speed of one wheel
void adjDir(char whl, char dir) ; // will adjust direction of one wheel
void turn(char dir, byte spd, short dly) ;

// Sensor Vars/Funcs
// 100 for emitter, 20k for receiver, i think smaller sensor val is farther
#define F_READ_PIN A0 // pin that reads ir
#define L_READ_PIN A1
#define R_READ_PIN A2
byte fEmitPin = 23; // pin that emits ir
byte lEmitPin = 23;
byte rEmitPin = 23;
const int minRead = 130 ;
const int rangeRead = 280 - minRead ;

int ambLight(int digpin, int anapin); // digpin is IR emitter, anapin is IR reader
int msrRaw(int digpin, int anapin);
int msrAdjusted(int digpin, int anapin);

// Interrupt Vars/Funcs
unsigned long systime ; // will remove this
byte lInterruptPin1 = 9 ;
byte lInterruptPin2 = 10 ;
byte rInterruptPin1 = 11 ;
byte rInterruptPin2 = 12 ;
double dist = 0 ;

void interruptRoutine() ;

// PID Vars/Funcs
// Kp * e[k] + Kd * (e[k] - e[k - 1]) / T + sum(e[k]), pos error will be too left, so turn right, vice versa
double prevErr = 0 ;
double sumErr = 0 ;
const double kp = 1, kd = 2 ;
unsigned long prevTime = 0 ;
const int pidAdj = 3 ;

void adjPID(double currErr, byte spd) ;

void setup() {
  Serial.begin(9600);
  pinMode(F_READ_PIN, INPUT);
  pinMode(fEmitPin, OUTPUT);
  /*pinMode(L_READ_PIN, INPUT); // will be for other sensors
  pinMode(lEmitPin, OUTPUT);
  pinMode(R_READ_PIN, INPUT);
  pinMode(rEmitPin, OUTPUT);*/
  pinMode(rightAnlgPin, OUTPUT) ;
  pinMode(rightDigPin1, OUTPUT) ;
  pinMode(rightDigPin2, OUTPUT) ;
  pinMode(leftAnlgPin, OUTPUT);
  pinMode(leftDigPin1, OUTPUT);
  pinMode(leftDigPin2, OUTPUT);
  pinMode(lInterruptPin1, INPUT) ;
  pinMode(lInterruptPin2, INPUT) ;
  pinMode(rInterruptPin1, INPUT) ;
  pinMode(rInterruptPin2, INPUT) ;
  attachInterrupt(digitalPinToInterrupt(lInterruptPin1), interruptRoutine, FALLING) ;
  attachInterrupt(digitalPinToInterrupt(lInterruptPin2), interruptRoutine, FALLING) ;
  attachInterrupt(digitalPinToInterrupt(rInterruptPin1), interruptRoutine, FALLING) ;
  attachInterrupt(digitalPinToInterrupt(rInterruptPin2), interruptRoutine, FALLING) ;
  interrupts() ;
  driveForward(leftSpeed, rightSpeed);
}

void loop() {
  //Serial.println("Raw IR: " + String(msrRaw(emit, READ_PIN)));
  //Serial.println("Ambient Light: " + String(ambLight(emit, READ_PIN)));
  //Serial.println("Adjusted IR: " + String(msrAdjusted(emitPin, READ_PIN)));

  int lSens = msrAdjusted(lEmitPin, L_READ_PIN) ;
  int rSens = msrAdjusted(rEmitPin, R_READ_PIN) ;
  int fSens = msrAdjusted(fEmitPin, F_READ_PIN) ;
  /* if (fSens > value) it's too close to a border
   *  decide what turn to prioritize
        if lSens < value
          do a turn one dir turn90
        else if rSens < value
          do a turn other dir
        if lSens > value and rSens > value
            turn with turn180
      lSens = msrAdjusted(lEmitPin, L_READ_PIN) ;
      rSens = msrAdjusted(rEmitPin, R_READ_PIN) ;
      fSens = msrAdjusted(fEmitPin, F_READ_PIN) ;*/
  int curSpeed = static_cast<int>(max(0, 1 - fSens) * 210) ;
  // TODO: formula for lsens and rsens to msr error
  // double err = lSens - rSens ;
  // adjPID(err, curSpeed
  // MAYDO: replace driveForward with adjPID?
  // driveForward(curSpeed, curSpeed) ;
}

void driveForward(int pwmVal1, int pwmVal2){
  adjDir('l', 'f') ;
  adjDir('r', 'f') ;
  analogWrite(leftAnlgPin, pwmVal1);
  analogWrite(rightAnlgPin, pwmVal2) ;
}

void adjSpeed(char dir, byte spd) {
  if (dir == 'l')
    analogWrite(leftAnlgPin, spd) ;
  else
    analogWrite(rightAnlgPin, spd) ;
}

void adjDir(char whl, char dir) {
  byte pin1, pin2 ;
  if (whl == 'l') {
    pin1 = leftDigPin1 ;
    pin2 = leftDigPin2 ;
  }
  else {
    pin1 = rightDigPin1 ;
    pin2 = rightDigPin2 ;
  }
  if (dir == 'f') {
    digitalWrite(pin1, HIGH) ;
    digitalWrite(pin2, LOW) ;
  }
  else {
    digitalWrite(pin1, LOW) ;
    digitalWrite(pin2, HIGH) ;
  }
}

void turn(char dir, byte spd, short dly) {
  driveForward(0, 0) ;
  delay(5) ;
  char fwrd, rvrs ;
  if (dir == 'l')  {
    rvrs = 'l' ;
    fwrd = 'r' ;
  }
  else {
    rvrs = 'r' ;
    fwrd = 'l' ;
  }

  adjDir(fwrd, 'f') ;
  adjDir(rvrs, 'b') ;
  adjSpeed(fwrd, spd) ;
  adjSpeed(rvrs, spd) ;
  delay(dly) ;
  driveForward(0, 0) ;
  delay(5) ;
  driveForward(20, 20) ;
  delay(5) ;
}
void interruptRoutine() {
  //systime = micros() ;
  //Serial.print("Interrupt: ") ;
  dist += 0.00057119866 ;
  //Serial.print(dist) ;
}

int ambLight(int digpin, int anapin)
{
  digitalWrite(digpin, LOW);
  delay(10);
  return analogRead(anapin);
}

int msrRaw(int digpin, int anapin)
{
  digitalWrite(digpin, HIGH);
  delay(10);
  return analogRead(anapin);
}

// MAYDO: meaure adjusted voltage or keep original units?
int msrAdjusted(int digpin, int anapin)
{
  int ambient = ambLight(digpin, anapin);
  int raw = msrRaw(digpin, anapin);
  int adjusted = raw - ambient;
  adjusted = (adjusted - minRead) / static_cast<double>(rangeRead) ;
  return adjusted;
}

void adjPID(double currErr, byte spd) {
  sumErr += currErr ;
  unsigned long currTime = micros() ;
  double pidErr = kp * currErr + kd * (currErr - prevErr) / (currTime - prevTime) + sumErr ;
  short newSpd = spd + static_cast<int>(pidErr) * pidAdj ;
  if (pidErr > 0) 
    driveForward(newSpd, spd) ;
  else 
    driveForward(spd, newSpd) ;
  delay(1) ;
  prevTime = currTime ;
  prevErr = currErr ;
}
