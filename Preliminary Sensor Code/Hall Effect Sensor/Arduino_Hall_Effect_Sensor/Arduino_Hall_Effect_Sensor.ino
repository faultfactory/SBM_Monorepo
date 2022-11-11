// BLACK WIRE IS OUTPUT

unsigned int hallSensorPin = 2;
unsigned int teeth = 32;
volatile unsigned long previousTime = 0;
volatile unsigned long startTime = 0;
double wheelSpeed = 0;

void setup() 
{
  Serial.begin(115200);
  pinMode(hallSensorPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(hallSensorPin), pulse, RISING);
}

void loop() 
{
  wheelSpeed = (double)((startTime - previousTime)/1000000.0); // Measures time between pulses in seconds (1 tooth/sec)
  Serial.println((1/(teeth*wheelSpeed))*60); // 
}

void pulse() // Determine time inbetween each pulse
{
  previousTime = startTime;
  startTime = micros();
}

/*
 * If pulse is called, read
 * Else, time = 0
*/
