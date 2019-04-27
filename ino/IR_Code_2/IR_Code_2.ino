#define READ_PIN A0
int emit = 23;
int ambient_lighting(int digpin, int anapin);
int measure_raw(int digpin, int anapin);
int measure_adjusted(int digpin, int anapin);
void setup() {
  Serial.begin(9600);
  pinMode(READ_PIN, INPUT);
  pinMode(emit, OUTPUT);
}

void loop() {
  Serial.println("Raw IR: " + String(measure_raw(emit, READ_PIN)));
  Serial.println("Ambient Light: " + String(ambient_lighting(emit, READ_PIN)));
  Serial.println("Adjusted IR: " + String(measure_adjusted(emit, READ_PIN)));
}
int ambient_lighting(int digpin, int anapin)
{
  digitalWrite(digpin, LOW);
  delay(100);
  return analogRead(anapin);
}
int measure_raw(int digpin, int anapin)
{
  digitalWrite(digpin, HIGH);
  delay(100);
  return analogRead(anapin);
}
int measure_adjusted(int digpin, int anapin)
{
  int ambient = ambient_lighting(digpin, anapin);
  int raw = measure_raw(digpin, anapin);
  int adjusted = raw - ambient;
  return adjusted;
}
