/* To be used as a suspension travel sensor */

const int PIN = A0;
int positionRaw;
int positionTrue;

void setup() {
  Serial.begin(115200);

}

void loop() {
  positionRaw = analogRead(A0);
  positionTrue = map(positionRaw, 97, 916, 0, 360);
  Serial.println(positionTrue);
}
