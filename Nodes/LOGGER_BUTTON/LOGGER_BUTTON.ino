#include <CAN.h>
#include <millisDelay.h>
#include "src/can_conv/sbm_network_definition.h"

can_obj_sbm_network_definition_h_t can_obj;

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

  Button_Press.start(50);
}

void loop() {
  if(Button_Press.justFinished())
  {
    Button_Press.repeat();

    bool buttonDown = false;
    if (buttonState != digitalRead(buttonPin) && digitalRead(buttonPin) == 1) {
      buttonDown = true;
  }
    buttonState = digitalRead(buttonPin);

    encode_can_0x104_CAN_Logger(&can_obj,1);
    encode_can_0x105_CAN_Logger(&can_obj,1); 

    MsgData msg_102, msg_104, msg_105;  

    //DEBUG
    double accel = 5.3;
    Serial.println(accel);
    encode_can_0x102_Raw_Accel_x_mps2(&can_obj,accel);
    pack_message(&can_obj,0x102,&msg_102.a);
    CAN.beginPacket(0x102,8,false);
    CAN.write(msg_102.b,8);
    CAN.endPacket();
    //END DEBUG
  
    /* Button pressed when logger is off */
    if (buttonDown & !Logger_Read){
      Logger_Read = true;
      
      Serial.println("Send logger START");
      pack_message(&can_obj,0x105,&msg_105.a);
      CAN.beginPacket(0x105,8,false);
      CAN.write(msg_105.b,8);
      CAN.endPacket();  
    }
    /* Button pressed when logger is on */
    else if (buttonDown & Logger_Read){
      Logger_Read = false;
      
      Serial.println("Send logger STOP");
      pack_message(&can_obj,0x104,&msg_104.a);
      CAN.beginPacket(0x104,8,false);
      CAN.write(msg_104.b,8);
      CAN.endPacket();  
    }
  }
}
