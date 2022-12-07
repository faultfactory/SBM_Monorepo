int linearpotPin = A0;
int raw, angle;

void setup() {
  Serial.begin(115200);

}

void loop() {
  raw = analogRead(linearpotPin);
  angle = raw - (1024/2);
  Serial.println(angle);

}
