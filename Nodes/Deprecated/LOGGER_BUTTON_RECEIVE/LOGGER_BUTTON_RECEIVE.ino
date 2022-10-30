#include <CAN.h>
#include <millisDelay.h>
#include "src/can_conv/sbm_network_definition.h"
#define SERIAL_DEBUG_PRINTS_ON 1
/*******************************************************************************/
uint8_t log_START, log_END;

// This defines the data storage object that the generated code us//s.
static can_obj_sbm_network_definition_h_t can_obj;

// Unions are special datatypes that provide two views of the same memmory location.
union MsgData
{
  uint64_t a;   // dbcc output uses uint64_t
  uint8_t b[8]; // Adafruit CAN library wants uint8_t[8]
};


millisDelay CAN_Send_Delay; 
void setup()
{
  /*******************************************************************************/
  pinMode(PIN_CAN_STANDBY, OUTPUT);
  digitalWrite(PIN_CAN_STANDBY, false); // turn off STANDBY
  pinMode(PIN_CAN_BOOSTEN, OUTPUT);
  digitalWrite(PIN_CAN_BOOSTEN, true); // turn on booster


  Serial.begin(115200);
  // start the CAN bus at 500 kbps (kilo-bits/second)
    CAN.begin(500000);

  CAN_Send_Delay.start(10); //10 ms -> 100 Hz (Receiving 2 messages at 100 Hz, so 200 messages/sec)
}

void loop()
{
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
        //Serial.println("ERROR: Packet shorter than specified"); // just some print outs if shit hits the fan.
        break;                                                  // something went wrong
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
      // LOGGER READ
      decode_can_0x104_CAN_Logger(&can_obj,&log_START);
      decode_can_0x105_CAN_Logger(&can_obj,&log_END); 
      
  
      /**********************************************************************************************/
      /* DEBUG PRINT LINES */
      #if SERIAL_DEBUG_PRINTS_ON
      // LOGGER data
      //Serial.print(log_START);Serial.print(" ");
      //Serial.print(log_END);Serial.print(" ");
      #endif
  /**********************************************************************************************/
    }
  }
    
  
}
