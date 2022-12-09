/* CAN-Bus transmit vehicle dynamics node*/

#include <CAN.h>
#include <millisDelay.h>
/*******************************************************************************/
// Timers
millisDelay CAN_Send_Delay; 
millisDelay Button_Press;

// Pressure transducer pins
const int analogInput1 = A0; //Analog input pin for pressure transducer #1
const int analogInput2 = A1; //Analog input pin for pressure transducer #2
const int resolution = 1024; //The arduino's analog voltage reading resolution in total units

// Transducer variables
const int numTrans = 2; //Number of transducers used
const float Vsupply = 3.0; //(V), Voltage supplied
const int Pmax = 500; //(psi), The maximum pressure the transducer can produce
const int Pmin = 0; //(psi), The minimum pressure the transducer can produce

//Reading & Calibration variables
float voltageReal[numTrans]; //Actual voltage being read
double Vmin[numTrans]; //Minimum voltage needed to achieve Pmin (0 psi at STP)
double Vmax = 0.9*Vsupply; //Maximum voltage needed to achieve Pmax (500 psi at STP)
float Papplied[numTrans]; //Calcualted pressure applied
float Vfilter[numTrans];
float Pfilter[numTrans];
float aLPF = 0.9; // Low pass filter weight

// Calibration button variables
bool Logger_Read = false;
int buttonState = 0;
const int buttonPin = 12;

void setup()
{
  /*******************************************************************************/
  pinMode(buttonPin, INPUT);
    
  pinMode(PIN_CAN_STANDBY, OUTPUT);
  digitalWrite(PIN_CAN_STANDBY, false); // turn off STANDBY
  pinMode(PIN_CAN_BOOSTEN, OUTPUT);
  digitalWrite(PIN_CAN_BOOSTEN, true); // turn on booster

  // Initialize arrays to zero
  voltageReal[0] = 0; //Actual voltage being read
  Vmin[0] = 0.1*Vsupply; //Minimum voltage needed to achieve Pmin (0 psi at STP)
  Papplied[0] = 0; //Calcualted pressure applied
  Vfilter[0] = 0;
  Pfilter[0] = 0;

  Serial.begin(115200);
  // start the CAN bus at 500 kbps
  CAN.begin(500000);
  
  CAN_Send_Delay.start(10); //10 ms -> 100 Hz (Sending 2 messages at 100 Hz, so 200 messages/sec)
  /*******************************************************************************/

}

void loop()
{
  /**********************************************************************************************/
  /********************************[BEGIN CAN-BUS PROTOCOL LOOP]**********************************/
  /**********************************************************************************************/
  if(CAN_Send_Delay.justFinished()) // Checking the delay on each loops
  {
    // Delay has expired so set it to run again. 
    // This library does not allow drift and logs the actual expiration time. 
    CAN_Send_Delay.repeat(); 
    
    // FILTER TESTING
    Vfilter[0] = aLPF*Vfilter[0] + (1 - aLPF)*analogRead(analogInput1); // Low pass filter (Voltage)
    voltageReal[0] = (Vsupply/resolution)*Vfilter[0];
    

    
  /**********************************************************************************************/
    // Re-calibrate the zero
    /* Check if button is pressed down */
    if(digitalRead(buttonPin) == 1) // Button is being pressed down
    {
      if(!buttonState) // If button IS NOT already pressed down, start timer
      {
        Button_Press.start(50);
        buttonState=true;
      }
      else // If button IS already pressed down, check if timer has finished
      {
        if(Button_Press.justFinished()) // If timer finished, re-calibrate pressure @ STP
        {
          Papplied[0] = 0;
          Vmin[0] = (Papplied[0]*Vmax - Pmin*Vmax - Pmax*voltageReal[0] + Pmin*voltageReal[0])/(Papplied[0] - Pmax);
          // Add for second transducer
        }
      }
    }
    else // Button is not being pressed down
    {
      if(buttonState) // Set button state to false if not already
      {
        buttonState = false;
        Button_Press.finish();      
      }
      else
      {
        if(Button_Press.isRunning()) // If button is lifted before timer is up, finish timer. NO CAN data is sent
        {
          Button_Press.finish();
        }
      }
    }
  /**********************************************************************************************/
    // Calculate pressure
    Papplied[0] = ((Pmax-Pmin)/(Vmax-Vmin[0]))*(voltageReal[0] - Vmin[0]) + Pmin;
    Pfilter[0] = aLPF*Pfilter[0] + (1 - aLPF)*Papplied[0]; // Low pass filter (Pressure), NOT SURE IF I SHOULD DO THIS AGAIN!!! [REMOVE? CHECK RESPONSE TIME]

    // Display pressure
    if (abs(Pfilter[0]) >= 0.3)
    {
       Serial.println(Pfilter[0]);
    }
    else
    {
      Serial.println(0);
    }
  }    
  
}
