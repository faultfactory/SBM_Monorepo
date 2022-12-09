const int Voutput = A0;

int Tare;
float Force, scale;

void setup() {
  pinMode(Voutput, INPUT);
  Serial.begin(115200);

}

void loop() {
  Tare = 12; // Update later w/ a button
  scale = 1.02; // User input?
  Force = float(analogRead(Voutput)-Tare)*scale;
  Serial.print(analogRead(Voutput)); Serial.print(", "); Serial.print(Force); Serial.println(" lbf");
}
