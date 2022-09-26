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
    //Serial.print("Received ");
    MsgData msg; // Create a union object to hold the incoming data.
    //Serial.print("packet with id 0x");
    long canID = CAN.packetId();
    //Serial.print(canID, HEX);
    //Serial.print(" and length ");
    //Serial.println((int)packetSize);

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
    double accelX_in, accelY_in, accelZ_in, gyroX_in, gyroY_in, gyroZ_in;
    int8_t magX_in, magY_in, magZ_in;
    
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
//    //Serial.println("Received data:");
//    // Acceleration data
//    Serial.print(accelX_in);Serial.print(" ");
//    Serial.print(accelY_in);Serial.print(" ");
//    Serial.print(accelZ_in);Serial.print(" ");
//    // Gyroscope data
//    Serial.print(gyroX_in);Serial.print(" ");
//    Serial.print(gyroY_in);Serial.print(" ");
//    Serial.print(gyroZ_in);Serial.print(" ");
//    // Magnetometer data
//    Serial.print(magX_in);Serial.print(" ");
//    Serial.print(magY_in);Serial.print(" ");
//    Serial.println(magZ_in);
  /**********************************************************************************************/
  }
    
  
}
