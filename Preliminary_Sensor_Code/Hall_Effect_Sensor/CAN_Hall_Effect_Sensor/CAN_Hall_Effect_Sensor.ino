/* CAN-Bus transmit vehicle dynamics node*/

#include <CAN.h>
#include <millisDelay.h>
/*******************************************************************************/
// Timers
millisDelay CAN_Send_Delay; 

void setup()
{
  /*******************************************************************************/
  pinMode(PIN_CAN_STANDBY, OUTPUT);
  digitalWrite(PIN_CAN_STANDBY, false); // turn off STANDBY
  pinMode(PIN_CAN_BOOSTEN, OUTPUT);
  digitalWrite(PIN_CAN_BOOSTEN, true); // turn on booster

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
    
  /**********************************************************************************************/

  // Read from I2C?
  
  }    
  
}
