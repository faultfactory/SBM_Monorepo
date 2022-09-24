#include <CAN.h>
#include <millisDelay.h>
#include "src/can_conv/sbm_network_definition.h"

#define SERIAL_DEBUG_PRINTS_ON 1

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
  pinMode(PIN_CAN_STANDBY, OUTPUT);
  digitalWrite(PIN_CAN_STANDBY, false); // turn off STANDBY
  pinMode(PIN_CAN_BOOSTEN, OUTPUT);
  digitalWrite(PIN_CAN_BOOSTEN, true); // turn on booster


  Serial.begin(115200);
  // NOTE: You'll need to get rid of this eventually
  while (!Serial); // THIS STOPS YOU FROM RUNNING UNTIL YOUR USB IS PLUGGED IN!
  // start the CAN bus at 500 kbps
    CAN.begin(500000);
}

void loop()
{
  // try to parse packet
  size_t packetSize = (size_t)CAN.parsePacket();
  if (packetSize)
  {
    // received a packet. We're not playing with RTR packets so let's not bother with that code
    Serial.print("Received ");
    MsgData msg; // Create a union object to hold the incoming data.
    Serial.print("packet with id 0x");
    long canID = CAN.packetId();
    Serial.print(canID, HEX);
    Serial.print(" and length ");
    Serial.println((int)packetSize);

    for (size_t i = 0; i < packetSize; i++)
    {
      if (!CAN.available())
      {
        Serial.println("ERROR: Packet shorter than specified"); // just some print outs if shit hits the fan.
        break;                                                  // something went wrong
      }
      msg.b[i] = (uint8_t)CAN.read();
    }
    unpack_message(&can_obj,canID,msg.a,(uint8_t)8,millis());
    
    double yaw_in, roll_in, pitch_in; 
    decode_can_0x100_Body_Pitch_deg(&can_obj,&pitch_in);
    decode_can_0x100_Body_Roll_deg(&can_obj,&roll_in);
    decode_can_0x100_Body_Yaw_deg(&can_obj,&yaw_in);
    
    Serial.print("Roll Recieved: ");
    Serial.println(roll_in);
    Serial.print("Pitch Recieved: ");
    Serial.println(pitch_in);
    Serial.print("Yaw Recieved: ");
    Serial.println(yaw_in);
    Serial.println();
  }
    
  
}
