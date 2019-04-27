// 100 for emitter, 68k for receiver
#define READ_PIN A0
int emit = 22;
int five_volt = 23;
const int minRead = 70 ;
const int rangeRead = 900 - minRead ;
int measure_Ambient(int digpin, int anapin);
int measure_Raw(int digpin, int anapin);
double measure_AdjustedVolt(int digpin, int anapin);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(READ_PIN,INPUT);
  pinMode(emit,OUTPUT);
  pinMode(five_volt,OUTPUT);
 
}
void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(five_volt,HIGH);
  Serial.println("Raw IR: " + String(measure_Raw(emit, READ_PIN)));
  //Serial.println("Ambient Light: " + String(measure_Ambient(emit, READ_PIN)));
  //Serial.println("Adjusted IR: " + String(measure_AdjustedVolt(emit, READ_PIN)));
}
int measure_Ambient(int digpin, int anapin)
{
  digitalWrite(digpin, LOW);
  delay(100);
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
  Serial.println("Raw IR: " + String(raw));
  Serial.println("Ambient Light: " + String(ambient));
  double adjusted = raw - ambient;
  //we can combine these two lines to optimize
  adjusted = (adjusted - minRead) / static_cast<double>(rangeRead) * 1023 ;
  adjusted = adjusted / 1023.0 * 5 ;
  return adjusted;
}

