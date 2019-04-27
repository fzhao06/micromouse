#define READ_PIN A0
int emit = 23;
unsigned long systime ;
int pwmVal = 40;
int pwmVal2 = 40 ;
int interruptPin1 = 9 ;
int interruptPin2 = 10 ;
double dist = 0 ;


void setup() {
  Serial.begin(9600) ;
  pinMode(4, OUTPUT) ;
  pinMode(5, OUTPUT) ;
  pinMode(6, OUTPUT) ;
  pinMode(3, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(READ_PIN, INPUT);
  pinMode(emit, OUTPUT);
  pinMode(interruptPin1, INPUT) ;
  pinMode(interruptPin2, INPUT) ;
  attachInterrupt(digitalPinToInterrupt(interruptPin1), interruptRoutine, FALLING) ;
  attachInterrupt(digitalPinToInterrupt(interruptPin1), interruptRoutine, FALLING) ;
  //interrupts() ;
  
  //driveForward(pwmVal, pwmVal2);
}

void driveForward(int pwmVal, int pwmVal2){
  analogWrite(3, pwmVal);
  digitalWrite(1, LOW);
  digitalWrite(2, HIGH);
  analogWrite(4, pwmVal2) ;
  digitalWrite(5, LOW) ;
  digitalWrite(6, HIGH) ;
}
int measure_raw(int digpin, int anapin)
{
  digitalWrite(digpin, HIGH);
  //delay(90);
  return analogRead(anapin);
}
void interruptRoutine() {
  systime = micros() ;
  Serial.print("Interrupt: ") ;
  dist += 0.00057119866 ;
  Serial.print(dist) ;
}

void loop() {
  // put your main code here, to run repeatedly:
  driveForward(pwmVal, pwmVal2) ;
  //digitalWrite(1, LOW) ;
  //digitalWrite(2, HIGH) ;
  //analogWrite(3, pwmVal) ;
  //if (pwmVal < 100) 
    //pwmVal++ ;
  //if (pwmVal2 < 100)
    //pwmVal2++ ;
  //Serial.println("Raw IR: " + String(measure_raw(emit, READ_PIN)));
  int sensor = measure_raw(emit, READ_PIN) ;
  pwmVal = max(0, static_cast<double>((150 - sensor)) / (190 - 60) * 150) ;
  pwmVal2 = pwmVal ;
  
  //delay(100);
}
