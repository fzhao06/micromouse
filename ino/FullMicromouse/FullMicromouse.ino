#include <math.h>

// Driving Vars/Funcs
// directions will be a character 'l' or not to represent left and right
byte leftAnlgPin = 3 ;
byte leftDigPin1 = 1 ;
byte leftDigPin2 = 2 ;
byte rightAnlgPin = 4 ;
byte rightDigPin1 = 5 ;
byte rightDigPin2 = 6 ;
byte leftSpeed = 15;
byte rightSpeed = 20 ;

void driveForward(int pwmVal1, int pwmVal2) ;
void adjSpeed(char dir, byte spd) ; // will adjust speed of one wheel
void adjDir(char whl, char dir) ; // will adjust direction of one wheel
void turn(char dir, byte spd) ;

// Sensor Vars/Funcs
// 100 for emitter, 68k for receiver
#define F_READ_PIN A0 // pin that reads ir
#define L_READ_PIN A1
#define R_READ_PIN A2
byte fEmitPin = 23; // pin that emits ir
//byte lEmitPin = 23;
//byte rEmitPin = 23;
const int minRead = 130 ;
const int rangeRead = 280 - minRead ;

int ambLight(int digpin, int anapin); // digpin is IR emitter, anapin is IR reader
int msrRaw(int digpin, int anapin);
int msrAdjusted(int digpin, int anapin);

// Interrupt Vars/Funcs
unsigned long systime ; // will remove this
byte interruptPin1 = 9 ;
byte interruptPin2 = 10 ;
double dist = 0 ;

void interruptRoutine() ;

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
  pinMode(interruptPin1, INPUT) ;
  pinMode(interruptPin2, INPUT) ;
  attachInterrupt(digitalPinToInterrupt(interruptPin1), interruptRoutine, FALLING) ;
  attachInterrupt(digitalPinToInterrupt(interruptPin1), interruptRoutine, FALLING) ;
  interrupts() ;
  driveForward(leftSpeed, rightSpeed);
}

void loop() {
  //Serial.println("Raw IR: " + String(msrRaw(emit, READ_PIN)));
  //Serial.println("Ambient Light: " + String(ambLight(emit, READ_PIN)));
  //Serial.println("Adjusted IR: " + String(msrAdjusted(emitPin, READ_PIN)));
  int curSpeed = static_cast<int>(max(0, 1 - msrAdjusted(emitPin, READ_PIN)) * 210) ;
  driveForward(curSpeed, curSpeed) ;
}

void driveForward(int pwmVal1, int pwmVal2){
  adjDir('l', 'f') ;
  adjDir('r', 'f') ;
  analogWrite(leftAnlgPin, leftSpeed);
  analogWrite(rightAnlgPin, rightSpeed) ;
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

void turn(char dir, byte spd) {
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
  delay(150) ;
  driveForward(0, 0) ;
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
