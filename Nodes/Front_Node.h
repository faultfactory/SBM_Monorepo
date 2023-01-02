/* CAN-Bus transmit vehicle dynamics node*/

#include <CAN.h>
#include <millisDelay.h>
#include "can_conv/sbm_network_definition.h"

// FOR OLED DISPLAY
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);

// Button assignment for M4 board
#define BUTTON_A  9
#define BUTTON_B  6
#define BUTTON_C  5

/*******************************************************************************/
// Timers
millisDelay CAN_Send_Delay; 
millisDelay Button_Press;

// Pressure transducer pins
const int analogInput1 = A0; //Analog input pin for pressure transducer #1
const int analogInput2 = A1; //Analog input pin for pressure transducer #2
const int resolution = 1024; //The arduino's analog voltage reading resolution in total units

// Steering sensor pins
const int analogInput3 = A2; // Analog input pin for rotary multi-turn pot

// Transducer variables
const int numTrans = 2; //Number of transducers used
const float Vsupply = 3.0; //(V), Voltage supplied
const int Pmax = 500; //(psi), The maximum pressure the transducer can produce
const int Pmin = 0; //(psi), The minimum pressure the transducer can produce

//Reading & Calibration variables
float anagIn[numTrans];
float voltageReal[numTrans]; //Actual voltage being read
double Vmin[numTrans]; //Minimum voltage needed to achieve Pmin (0 psi at STP)
double Vmax = 0.9*Vsupply; //Maximum voltage needed to achieve Pmax (500 psi at STP)
float Papplied[numTrans]; //Calcualted pressure applied
float Vfilter[numTrans];
float Pfilter[numTrans];
float aLPF = 0.9; // Low pass filter weight

// Steering
float steerRaw, steerVal;

// Calibration button variables
int buttonState = 0;
const int buttonPin = BUTTON_A;

// CAN bus
/* This defines the data storage object that the generated code us//s */
static can_obj_sbm_network_definition_h_t can_obj;

// Unions are special datatypes that provide two views of the same memmory location.
union MsgData
{
  uint64_t a;   // dbcc output uses uint64_t
  uint8_t b[8]; // Adafruit CAN library wants uint8_t[8]
};

/*******************************************************************************/
// OLED DISPLAY DEBUGGER


/*******************************************************************************/

void setup()
{
  /*******************************************************************************/
  pinMode(buttonPin, INPUT);

  // Setup M4 CAN board
  pinMode(PIN_CAN_STANDBY, OUTPUT);
  digitalWrite(PIN_CAN_STANDBY, false); // turn off STANDBY
  pinMode(PIN_CAN_BOOSTEN, OUTPUT);
  digitalWrite(PIN_CAN_BOOSTEN, true); // turn on booster

  // Setup OLED Display
  display.begin(0x3C, true); // Address 0x3C default
  
  display.clearDisplay();
  display.display();
  
  display.setRotation(1);
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);
  
  
  // Initialize arrays to zero
  for (int i = 0; i < numTrans; i++) {
    voltageReal[i] = 0; //Actual voltage being read
    Vmin[i] = 0.1*Vsupply; //Minimum voltage needed to achieve Pmin (0 psi at STP)
    Papplied[i] = 0; //Calcualted pressure applied
    Vfilter[i] = 0;
    Pfilter[i] = 0;
  }

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

    anagIn[0] = analogRead(analogInput1);
    anagIn[1] = analogRead(analogInput2);
    
    steerRaw = analogRead(analogInput3);
    steerVal = map(steerRaw,182,1003,0,270);
    
    // FILTER TESTING
    for (int i = 0; i < numTrans; i++) {
      Vfilter[i] = aLPF*Vfilter[i] + (1 - aLPF)*anagIn[i]; // Low pass filter (Voltage)
      voltageReal[i] = (Vsupply/resolution)*Vfilter[i];
    }
    

    
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
          for (int i = 0; i < numTrans; i++) {
            Papplied[i] = 0;
            Vmin[i] = (Papplied[i]*Vmax - Pmin*Vmax - Pmax*voltageReal[i] + Pmin*voltageReal[i])/(Papplied[i] - Pmax);
          }
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
    for (int i = 0; i < numTrans; i++) {
      Papplied[i] = ((Pmax-Pmin)/(Vmax-Vmin[i]))*(voltageReal[i] - Vmin[i]) + Pmin;
    }

     // Print information to serial monitor & OLED
     Serial.print(Papplied[0]); Serial.print(" "); Serial.println(Papplied[1]);
     display.clearDisplay();
     display.setCursor(0,0);
     display.setCursor(0,0); display.println("Front:"); display.setCursor(85,0); display.println("psi");
     display.setCursor(0,10); display.println("Rear:"); display.setCursor(85,10); display.println("psi");
     display.setCursor(0,20); display.println("Bias:"); display.setCursor(85,20); display.println("%");
     display.setCursor(0,30); display.println("Steer:"); display.setCursor(85,30); display.println("deg");
     display.setCursor(40,0); display.println(Papplied[0]);
     display.setCursor(40,10); display.println(Papplied[1]);
     display.setCursor(40,20); display.println((Papplied[0]/(Papplied[0] + Papplied[1]))*100);
     display.setCursor(40,30); display.println(steerRaw);
     display.display();  
     /**********************************************************************************************/
     // PRESSURE TRANSDUCER DATA
     encode_can_0x099_FL_Press_psi(&can_obj,Pfilter[0]);
     encode_can_0x099_FR_Press_psi(&can_obj,Pfilter[1]);
     
     // This is the union datatype that allows us to read in uint64_t or uint8_t[8]
     MsgData msg_099;

     // This command unloads the message
     pack_message(&can_obj,0x099,&msg_099.a);
     CAN.beginPacket(0x099,8,false);
     CAN.write(msg_099.b,8);
     CAN.endPacket();  
  }
}
