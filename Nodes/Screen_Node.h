#include <CAN.h>
#include <millisDelay.h>
#include "can_conv/sbm_network_definition.h"
#define SERIAL_DEBUG_PRINTS_ON 1
/*******************************************************************************/

// Declare IMU variables
double accelX_in, accelY_in, accelZ_in, gyroX_in, gyroY_in, gyroZ_in, pressFL_in, pressFR_in;
int8_t magX_in, magY_in, magZ_in;

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
  pinMode(buttonPin, INPUT);
  
  pinMode(PIN_CAN_STANDBY, OUTPUT);
  digitalWrite(PIN_CAN_STANDBY, false); // turn off STANDBY
  pinMode(PIN_CAN_BOOSTEN, OUTPUT);
  digitalWrite(PIN_CAN_BOOSTEN, true); // turn on booster


  Serial.begin(115200);
  CAN.begin(500000); // start the CAN bus at 500 kbps (kilo-bits/second)

  CAN_Send_Delay.start(10); //10 ms -> 100 Hz (Receiving 2 messages at 100 Hz, so 200 messages/sec)
}

void sendCAN(can_obj_sbm_network_definition_h_t &can_obj, bool &Logger_Read);
void loop()
{
  /**********************************************************************************************/
  // try to parse packet
  size_t packetSize = (size_t)CAN.parsePacket();
  if (packetSize)
  {
    MsgData msg; // Create a union object to hold the incoming data.
    long canID = CAN.packetId();

//    #if SERIAL_DEBUG_PRINTS_ON
//    Serial.print("Received ");
//    Serial.print("packet with id 0x");
//    Serial.print(canID, HEX);
//    Serial.print(" and length ");
//    Serial.println((int)packetSize);
//    #endif

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
  
      /**********************************************************************************************/
      /* DEBUG PRINT LINES */
      #if SERIAL_DEBUG_PRINTS_ON
      // Pressure transducer data
      Serial.print(pressFL_in);Serial.print(" ");
      Serial.print(pressFR_in);Serial.print(" ");
      // Acceleration data
      Serial.print(accelX_in);Serial.print(" ");
      Serial.print(accelY_in);Serial.print(" ");
      Serial.print(accelZ_in);Serial.print(" ");
      // Gyroscope data
      Serial.print(gyroX_in);Serial.print(" ");
      Serial.print(gyroY_in);Serial.print(" ");
      Serial.print(gyroZ_in);Serial.print(" ");
      // Magnetometer data
      Serial.print(magX_in);Serial.print(" ");
      Serial.print(magY_in);Serial.print(" ");
      Serial.println(magZ_in);
      #endif
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
