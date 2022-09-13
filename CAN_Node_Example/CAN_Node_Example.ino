#include <CAN.h>
#include <millisDelay.h>
#include "src/can_conv/sbm_network_definition.h"

// Eventually you'll want to wrap the debugging printouts in 
//  preprocessor directves so they are easily shut off. 
#define SERIAL_DEBUG_PRINTS_ON 1
/* 
Example: 

#if SERIAL_DEBUG_PRINTS_ON
Serial.println("Userful but costly information").
#endif

*/ 

// This defines the data storage object that the generated code us//s.
static can_obj_sbm_network_definition_h_t can_obj;

// here's some static storage for fake generated IMU signals
static float yaw = 0.0f;
static float pitch = 0.0; 
static float roll = 0.0f; 

void makeRandomIMUValues()
{
  float millisFloat = (float)millis();
  yaw = 180.f + 180.0f*sin((HALF_PI+millisFloat)/TWO_PI);
  pitch = 180.f + 180.0f*sin((PI+millisFloat)/TWO_PI);
  roll = 180.f + 180.0f*sin((EULER+millisFloat)/TWO_PI);
}

// Unions are special datatypes that provide two views of the same memmory location.
union MsgData
{
  uint64_t a;   // dbcc output uses uint64_t
  uint8_t b[8]; // Adafruit CAN library wants uint8_t[8]
};

const uint64_t zeros = 0;

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
  Serial.println("CAN Loopback Demonstration");
  // start the CAN bus at 500 kbps
    CAN.begin(500000);

  ///////////EXAMPLE ONLY, DELETE ME FOR USE ON VEHICLE //////////
  //CAN Loopback is for validating code with a single board. Remove this for a multi-node network
  CAN.loopback();
  ///////////EXAMPLE ONLY, DELETE ME FOR USE ON VEHICLE //////////

  // creating a millsDelay object using the library <millsDelay.h>
  // This provides non-blocking delays that don't obstruct execution
  // like using the standard delay() function would. 

  CAN_Send_Delay.start(1000); // We want to execute after every 1000 milliseconds
}

void loop()
{
  if(CAN_Send_Delay.justFinished()) // Checking the delay on each loops
  {
    // Delay has expired so set it to run again. 
    // This library does not allow drift and logs the actual expiration time. 
    CAN_Send_Delay.repeat(); 
    Serial.println(millis());
    Serial.println("The millis() output is printed above because to demonstrate that the internal time is right, despite the serial timestamps being wrong.");
    makeRandomIMUValues();
    Serial.println("Sending the following values through CAN");
    Serial.print("Roll:" ); Serial.println(roll);
    Serial.print("Pitch:"); Serial.println(pitch);
    Serial.print("Yaw:"  ); Serial.println(yaw);

    // encoding each value into the 64 bit can message per the 
    // bit structure, scaling and offset defined in the .dbc file
    // this puts the data into storage in the can_obj variable.
    // its datatype was created by dbcc and encompaseses the whole .dbc
    encode_can_0x100_Body_Roll_deg(&can_obj,roll); 
    encode_can_0x100_Body_Pitch_deg(&can_obj,pitch);
    encode_can_0x100_Body_Yaw_deg(&can_obj,yaw);
    
    // This is the union datatype that allows us to read in uint64_t or uint8_t[8]
    MsgData msg;  

    // This command unloads the message
    pack_message(&can_obj,0x100,&msg.a);

    CAN.beginPacket(0x100,8,false);
    CAN.write(msg.b,8);
    CAN.endPacket();  

    // After sending the message we can go ahead and encode zeros across
    // the entire storage structure to give ourselves more confidence that
    // the code is doing what we expect. This is not required. just a demonstration
    // that we're not looking at our old values. 
    encode_can_0x100_Body_Roll_deg(&can_obj,0.0f); 
    encode_can_0x100_Body_Pitch_deg(&can_obj,0.0f);
    encode_can_0x100_Body_Yaw_deg(&can_obj,0.0f);
  }

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
