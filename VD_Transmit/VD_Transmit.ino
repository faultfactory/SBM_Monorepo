/* CAN-Bus transmit vehicle dynamics node*/

#include <CAN.h>
#include <millisDelay.h>
#include "src/sbm_network_definition.h"

#define SERIAL_DEBUG_PRINTS_ON 1

/*******************************************************************************/

#include <Adafruit_LSM6DSOX.h>
Adafruit_LSM6DSOX lsm6ds;

#include <Adafruit_LIS3MDL.h>
Adafruit_LIS3MDL lis3mdl;

/*******************************************************************************/

static float roll = 0.0f;
static float pitch = 0.0f;
static float yaw = 0.0f;

/* This defines the data storage object that the generated code us//s */
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
  // NOTE: You'll need to get rid of this eventually
  while (!Serial); // THIS STOPS YOU FROM RUNNING UNTIL YOUR USB IS PLUGGED IN!
  // start the CAN bus at 500 kbps
    CAN.begin(500000);

  // creating a millsDelay object using the library <millsDelay.h>
  // This provides non-blocking delays that don't obstruct execution
  // like using the standard delay() function would. 
  CAN_Send_Delay.start(10); // We want to execute after every 1000 milliseconds
  
  /*******************************************************************************/
   bool lsm6ds_success, lis3mdl_success;

  /* Hardware I2C mode, can pass in address & alt wire */
  lsm6ds_success = lsm6ds.begin_I2C();
  lis3mdl_success = lis3mdl.begin_I2C();

  if (!lsm6ds_success){
    Serial.println("Failed to find LSM6DS chip");
  }
  if (!lis3mdl_success){
    Serial.println("Failed to find LIS3MDL chip");
  }
  
  Serial.println("LSM6DS and LIS3MDL Found!");
  
  /*******************************************************************************/
  /* Configure accelerometer range */
  lsm6ds.setAccelRange(LSM6DS_ACCEL_RANGE_4_G);

  /* Configure accelerometer data rate */
  lsm6ds.setAccelDataRate(LSM6DS_RATE_208_HZ);

  /* Configure gyro range */
  lsm6ds.setGyroRange(LSM6DS_GYRO_RANGE_2000_DPS);

  /* Configure gyro data rate */
  lsm6ds.setGyroDataRate(LSM6DS_RATE_208_HZ);

  /* Configure magnetometer data rate */
  lis3mdl.setDataRate(LIS3MDL_DATARATE_155_HZ);
  // You can check the datarate by looking at the frequency of the DRDY pin

  /* Configure magnetometer data range */
  lis3mdl.setRange(LIS3MDL_RANGE_4_GAUSS);

  /* Configure magnetometer performance mode */
  lis3mdl.setPerformanceMode(LIS3MDL_MEDIUMMODE);

  /* Configure magnetometer operation mode */
  lis3mdl.setOperationMode(LIS3MDL_CONTINUOUSMODE);

  lis3mdl.setIntThreshold(500);
  lis3mdl.configInterrupt(false, false, true, // enable z axis
                          true, // polarity
                          false, // don't latch
                          true); // enabled!
                          
  /*******************************************************************************/
}

void loop()
{
  /**********************************************************************************************/
  /*************************************[BEGIN IMU LOOP]*****************************************/
  /**********************************************************************************************/
  sensors_event_t accel, gyro, mag, temp;

  /* Get new normalized sensor events */
  lsm6ds.getEvent(&accel, &gyro, &temp);
  lis3mdl.getEvent(&mag);

  /**********************************************************************************************/
  /********************************[BEGIN CAN-BUS PROTOCOL LOOP]**********************************/
  /**********************************************************************************************/
  if(CAN_Send_Delay.justFinished()) // Checking the delay on each loops
  {
    // Delay has expired so set it to run again. 
    // This library does not allow drift and logs the actual expiration time. 
    CAN_Send_Delay.repeat(); 

    /* TEMPORARY. Need to update dbc file to incorporate acceleration values */
    roll = accel.acceleration.x;
    pitch = accel.acceleration.y;
    yaw = accel.acceleration.z;
    
    Serial.println("Sending the following values through CAN");
    Serial.print("Accel X: "); Serial.println(roll);
    Serial.print("Accel Y: "); Serial.println(pitch);
    Serial.print("Accel Z: "); Serial.println(yaw);

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
  
}
