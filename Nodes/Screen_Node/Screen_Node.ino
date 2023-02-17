#include <CAN.h>
#include <millisDelay.h>
#include "src/can_conv/sbm_network_definition.h"
#define SERIAL_DEBUG_PRINTS_ON 1
/*******************************************************************************/

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

// Declare IMU variables
double accelX_in, accelY_in, accelZ_in, gyroX_in, gyroY_in, gyroZ_in, pressFL_in, pressFR_in, Vg_in, psi_in;
int8_t magX_in, magY_in, magZ_in;
double RPM_1, RPM_2;

// Declare button variables
bool Logger_Read = false;
int buttonState = 0;
const int buttonPin = 12;

// Declare delay variables
millisDelay CAN_Send_Delay; 
millisDelay Button_Press; 

// This defines the data storage object that the generated code us//s.
static can_obj_sbm_network_definition_h_t can_obj;

// Unions are special datatypes that provide two views of the same memmory location.
union MsgData
{
  uint64_t a;   // dbcc output uses uint64_t
  uint8_t b[8]; // Adafruit CAN library wants uint8_t[8]
};

void setup()
{
  /*******************************************************************************/
  Serial.begin(115200);
  
  pinMode(buttonPin, INPUT);
  
  pinMode(PIN_CAN_STANDBY, OUTPUT);
  digitalWrite(PIN_CAN_STANDBY, false); // turn off STANDBY
  pinMode(PIN_CAN_BOOSTEN, OUTPUT);
  digitalWrite(PIN_CAN_BOOSTEN, true); // turn on booster

  // Setup OLED Display
  display.begin(0x3C, true); // Address 0x3C default

  display.display();
  delay(1000);
  display.clearDisplay();
  display.display();

  display.setRotation(1);
  
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);

  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0,0);
  display.print(" Waiting for CAN \ninput . . .");
  display.display(); 
  
  CAN.begin(500000); // start the CAN bus at 500 kbps (kilo-bits/second)

  CAN_Send_Delay.start(10); //10 ms -> 100 Hz (Receiving 2 messages at 100 Hz, so 200 messages/sec)
}

void loop()
{
  /**********************************************************************************************/
  // try to parse packet
  size_t packetSize = (size_t)CAN.parsePacket();
  if (packetSize)
  {
    MsgData msg; // Create a union object to hold the incoming data.
    long canID = CAN.packetId();

    #if SERIAL_DEBUG_PRINTS_ON
    Serial.print("Received ");
    Serial.print("packet with id 0x");
    Serial.print(canID, HEX);
    Serial.print(" and length ");
    Serial.println((int)packetSize);
    #endif

    for (size_t i = 0; i < packetSize; i++)
    {
      if (!CAN.available())
      {
        #if SERIAL_DEBUG_PRINTS_ON
        //Serial.println("ERROR: Packet shorter than specified"); // just some print outs if shit hits the fan.
        #endif                                                    // something went wrong
        
        break;
      }
      msg.b[i] = (uint8_t)CAN.read();
    }
    unpack_message(&can_obj,canID,msg.a,(uint8_t)8,millis());
    
    /**********************************************************************************************/
    if(CAN_Send_Delay.justFinished()) // Checking the delay on each loops
    {      
      // Delay has expired so set it to run again. 
      // This library does not allow drift and logs the actual expiration time. 
      CAN_Send_Delay.repeat(); 

      /* Check if button is pressed down */
      if(digitalRead(buttonPin) == 1) // Button is being pressed down
      {
        if(!buttonState) // If button IS NOT already pressed down, start timer
        {
          Button_Press.start(5);
          buttonState = true;
        }
        else // If button IS already pressed down, check if timer has finished
        {
          if(Button_Press.justFinished()) // If timer finished, send CAN data
          {
             sendCAN(can_obj, Logger_Read);
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
        else // 
        {
          if(Button_Press.isRunning()) // If button is lifted before timer is up, finish timer. NO CAN data is sent
          {
            Button_Press.finish();
          }
        }
      }

      // PRESSURE TRANSDUCER DATA
      decode_can_0x099_FL_Press_psi(&can_obj,&pressFL_in);
      decode_can_0x099_FR_Press_psi(&can_obj,&pressFR_in);

      // GPS READ
      decode_can_0x101_Body_Speed_mps(&can_obj,&Vg_in);
      decode_can_0x101_True_Yaw_deg(&can_obj,&psi_in);
      
      // ACCELERATION READ
      decode_can_0x102_Raw_Accel_x_mps2(&can_obj,&accelX_in); 
      decode_can_0x102_Raw_Accel_y_mps2(&can_obj,&accelY_in);
      decode_can_0x102_Raw_Accel_z_mps2(&can_obj,&accelZ_in);
  
      // GYROSCOPE DATA
      decode_can_0x102_Raw_Gyro_x_rps(&can_obj,&gyroX_in); 
      decode_can_0x102_Raw_Gyro_y_rps(&can_obj,&gyroY_in);
      decode_can_0x102_Raw_Gyro_z_rps(&can_obj,&gyroZ_in);
  
      // MAGNETOMETER DATA
      decode_can_0x103_Raw_Mag_x_uT(&can_obj,&magX_in); 
      decode_can_0x103_Raw_Mag_y_uT(&can_obj,&magY_in);
      decode_can_0x103_Raw_Mag_z_uT(&can_obj,&magZ_in);

      // WHEEL SPEED
      decode_can_0x098_FL_Wheelspeed_rpm(&can_obj, &RPM_1);
      decode_can_0x098_FR_Wheelspeed_rpm(&can_obj, &RPM_2);
  
      /**********************************************************************************************/
     // Print information to serial monitor & OLED
     display.clearDisplay();
     display.setCursor(0,0);
     display.setCursor(0,0); display.println("Front:"); display.setCursor(85,0); display.println("psi");
     display.setCursor(0,10); display.println("Rear:"); display.setCursor(85,10); display.println("psi");
     display.setCursor(0,20); display.println("Steer:"); display.setCursor(85,20); display.println("deg");
     display.setCursor(0,30); display.println("Brake:"); display.setCursor(85,30); display.println("lbf");
     display.setCursor(0,40); display.println("Hall_1:"); display.setCursor(85,40); display.println("rpm");
     display.setCursor(0,50); display.println("Hall_2:"); display.setCursor(85,50); display.println("rpm");
     display.setCursor(40,40); display.println(RPM_1);
     display.setCursor(40,50); display.println(RPM_2);
     display.display();  
  /**********************************************************************************************/
    }
  }
}

void sendCAN(can_obj_sbm_network_definition_h_t &can_obj, bool &Logger_Read) 
{
  encode_can_0x104_CAN_Logger(&can_obj,1);
  encode_can_0x105_CAN_Logger(&can_obj,1); 

  MsgData msg_104, msg_105;  

  /* Button pressed when logger is off */
  if (!Logger_Read){
    Logger_Read = true;

    #if SERIAL_DEBUG_PRINTS_ON
    Serial.println("Send logger START");
    #endif
    
    pack_message(&can_obj,0x105,&msg_105.a);
    CAN.beginPacket(0x105,8,false);
    CAN.write(msg_105.b,8);
    CAN.endPacket();  
  }
  /* Button pressed when logger is on */
  else if (Logger_Read){
    Logger_Read = false;

    #if SERIAL_DEBUG_PRINTS_ON
    Serial.println("Send logger STOP");
    #endif
    
    pack_message(&can_obj,0x104,&msg_104.a);
    CAN.beginPacket(0x104,8,false);
    CAN.write(msg_104.b,8);
    CAN.endPacket();  
  }
}
