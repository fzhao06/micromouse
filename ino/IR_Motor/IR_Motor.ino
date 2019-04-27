#include <math.h>
unsigned long systime ;
int pwmVal = 15;
int pwmVal2 = 20 ;
int interruptPin1 = 9 ;
int interruptPin2 = 10 ;
double dist = 0 ;
// 100 for emitter, 68k for receiver
#define READ_PIN A0
int emit = 22;
int five_volt = 23;
const int minRead = 130 ;
const int rangeRead = 280 - minRead ;
int measure_Ambient(int digpin, int anapin);
int measure_Raw(int digpin, int anapin);
double measure_AdjustedVolt(int digpin, int anapin);

void driveForward(int pwmVal, int pwmVal2){
  analogWrite(3, pwmVal);
  digitalWrite(1, HIGH);
  digitalWrite(2, LOW);
  analogWrite(4, pwmVal2) ;
  digitalWrite(5, HIGH) ;
  digitalWrite(6, LOW) ;
}
void interruptRoutine() {
  systime = micros() ;
  //Serial.print("Interrupt: ") ;
  dist += 0.00057119866 ;
  //Serial.print(dist) ;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(READ_PIN,INPUT);
  pinMode(emit,OUTPUT);
  pinMode(five_volt,OUTPUT);

  pinMode(4, OUTPUT) ;
  pinMode(5, OUTPUT) ;
  pinMode(6, OUTPUT) ;
  pinMode(3, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(interruptPin1, INPUT) ;
  pinMode(interruptPin2, INPUT) ;
  attachInterrupt(digitalPinToInterrupt(interruptPin1), interruptRoutine, FALLING) ;
  attachInterrupt(digitalPinToInterrupt(interruptPin1), interruptRoutine, FALLING) ;
  interrupts() ;
  driveForward(pwmVal, pwmVal2);
 
}
void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(five_volt,HIGH);
  double temp = max(0, 1 - measure_AdjustedVolt(five_volt, READ_PIN)) * 210 ;
  driveForward(static_cast<int>(temp), pwmVal2) ;
  //measure_AdjustedV(five_volt, READ_PIN) ;
  //Serial.println("Ambient Light: " + String(measure_Ambient(five_volt, READ_PIN)));
  //Serial.println("Adjusted IR: " + String(measure_AdjustedVolt(five_volt, READ_PIN)));
}
int measure_Ambient(int digpin, int anapin)
{
  digitalWrite(digpin, LOW);
  //delay(100);
  return analogRead(anapin);
}
int measure_Raw(int digpin, int anapin) {
  digitalWrite(digpin, HIGH);
  //delay(100);
  return analogRead(anapin);
}
double measure_AdjustedVolt(int digpin, int anapin) {
  int ambient = measure_Ambient(digpin, anapin);
  int raw = measure_Raw(digpin, anapin);
  //Serial.println("Raw IR: " + String(raw));
  //Serial.println("Ambient Light: " + String(ambient));
  double adjusted = raw - ambient;
  //we can combine these two lines to optimize
  adjusted = (adjusted - minRead) / static_cast<double>(rangeRead) ;//* 1023 ;
  //adjusted = adjusted / 1023.0 * 5 ;
  return adjusted;
}
