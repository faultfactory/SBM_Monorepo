#include <CAN.h>
#include <millisDelay.h>
#include "src/can_conv/sbm_network_definition.h"

can_obj_sbm_network_definition_h_t can_obj;

millisDelay CAN_Send_Delay; 
millisDelay Button_Press; 
bool Logger_Read = false;
int buttonState = 0;
const int buttonPin = 12;

union MsgData
{
  uint64_t a;   // dbcc output uses uint64_t
  uint8_t b[8]; // Adafruit CAN library wants uint8_t[8]
};

void setup() {
  pinMode(buttonPin, INPUT);
  
  pinMode(PIN_CAN_STANDBY, OUTPUT);
  digitalWrite(PIN_CAN_STANDBY, false); // turn off STANDBY
  pinMode(PIN_CAN_BOOSTEN, OUTPUT);
  digitalWrite(PIN_CAN_BOOSTEN, true); // turn on booster
  
  Serial.begin(115200);
  CAN.begin(500000); // start the CAN bus at 500 kbps (kilo-bits/second)

  CAN_Send_Delay.start(10); //10 ms -> 100 Hz (Receiving 2 messages at 100 Hz, so 200 messages/sec)
}

void loop() 
{
  if(CAN_Send_Delay.justFinished()) // Checking the delay on each loops
  {
      // Delay has expired so set it to run again. 
      // This library does not allow drift and logs the actual expiration time. 
      CAN_Send_Delay.repeat(); 

    //DEBUG
    MsgData msg_102;
    double accel = 5.3;
    Serial.println(accel);
    encode_can_0x102_Raw_Accel_x_mps2(&can_obj,accel);
    pack_message(&can_obj,0x102,&msg_102.a);
    CAN.beginPacket(0x102,8,false);
    CAN.write(msg_102.b,8);
    CAN.endPacket();
    //END DEBUG

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
    
    Serial.println("Send logger START");
    pack_message(&can_obj,0x105,&msg_105.a);
    CAN.beginPacket(0x105,8,false);
    CAN.write(msg_105.b,8);
    CAN.endPacket();  
  }
  /* Button pressed when logger is on */
  else if (Logger_Read){
    Logger_Read = false;
    
    Serial.println("Send logger STOP");
    pack_message(&can_obj,0x104,&msg_104.a);
    CAN.beginPacket(0x104,8,false);
    CAN.write(msg_104.b,8);
    CAN.endPacket();  
  }
}
