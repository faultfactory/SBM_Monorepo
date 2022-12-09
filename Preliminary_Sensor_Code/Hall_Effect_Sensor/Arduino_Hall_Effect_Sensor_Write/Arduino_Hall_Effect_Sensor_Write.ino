/* Wires:
 *  Red: Vsupply
 *  Black: Ground
 *  White & Black: Digital out
 */

#include <Wire.h>

// I2C Variables
const int msgSize = 4;
byte msg[msgSize]; // Message in bytes (8 maximum)
uint16_t RPM_1, RPM_2;

// Hall Effect Sensor Variables
unsigned int wheel_RIGHT_pin = 2; // Right wheel
unsigned int wheel_LEFT_pin = 3; // Left wheel

unsigned const int numSensors = 2;
unsigned int teeth = 32; // Change according to number of teeth on rotor or whatever

volatile unsigned long previousTime[numSensors];
volatile unsigned long startTime[numSensors];
uint16_t wheelSpeed[numSensors];


void setup() 
{
  Serial.begin(115200);

  // I2C Setup
  Wire.begin(54);
  Wire.onRequest(requestEvent);

  // Set up hall effect sensor pins
  pinMode(wheel_RIGHT_pin, INPUT_PULLUP);
  pinMode(wheel_LEFT_pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(wheel_RIGHT_pin), pulseRIGHT, RISING);
  attachInterrupt(digitalPinToInterrupt(wheel_LEFT_pin), pulseLEFT, RISING);

  // Initialize all array elements to zero
  for (int i = 0; i < numSensors; i++) {
    previousTime[i] = 0;
    startTime[i] = 0;
    wheelSpeed[i] = 0;
  }
}

void loop() 
{
  // Nothing
}


/* PULSE FUNCTIONS FOR EACH INDIVIDUAL WHEEL */
void pulseRIGHT() // RIGHT --> 0
{
  previousTime[0] = startTime[0];
  startTime[0] = micros();
}
void pulseLEFT() // LEFT --> 1
{
  previousTime[1] = startTime[1];
  startTime[1] = micros();
}

// I2C Functions
void requestEvent()
{
  // Calculate wheelspeed for each wheel (Measures time between pulses in seconds (1 tooth/sec))
  wheelSpeed[0] = int(60/(teeth*((double)((startTime[0] - previousTime[0])/1000000.0))));
  wheelSpeed[1] = int(60/(teeth*((double)((startTime[1] - previousTime[1])/1000000.0))));

//  //Print results
  Serial.print(wheelSpeed[0]); Serial.print(" ");
  Serial.println(wheelSpeed[1]);

  // Write numbers into message
  write_msg(wheelSpeed[0], msg, 0);
  write_msg(wheelSpeed[1], msg, 2);

  // Send off via I2C
  Wire.write(msg, 4);
}

void write_msg(uint16_t x, byte *Arr, int start_bit) {
  Arr[start_bit] = (x >> 8) & 0xFF;
  Arr[++start_bit] = x & 0xFF;
}
