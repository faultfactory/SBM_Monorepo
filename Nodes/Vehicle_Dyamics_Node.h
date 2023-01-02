/* CAN-Bus transmit vehicle dynamics node*/

#include <CAN.h>
#include <millisDelay.h>
#include <Adafruit_LSM6DSOX.h>
#include <Adafruit_LIS3MDL.h>
#include "can_conv/sbm_network_definition.h"
#define SERIAL_DEBUG_PRINTS_ON 1
/*******************************************************************************/

// Create IMU objects
Adafruit_LSM6DSOX lsm6ds;
Adafruit_LIS3MDL lis3mdl;
bool lsm6ds_success, lis3mdl_success;
float accelX, accelY, accelZ, gyroX, gyroY, gyroZ, magX, magY, magZ;
float aLPF = 0.9; // Low pass filter weight

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
  // start the CAN bus at 500 kbps
    CAN.begin(500000);

  // creating a millsDelay object using the library <millsDelay.h>
  // This provides non-blocking delays that don't obstruct execution
  // like using the standard delay() function would. 
  CAN_Send_Delay.start(10); //10 ms -> 100 Hz (Sending 2 messages at 100 Hz, so 200 messages/sec)
  
  /*******************************************************************************/
 
  /* Hardware I2C mode, can pass in address & alt wire */
  lsm6ds_success = lsm6ds.begin_I2C();
  lis3mdl_success = lis3mdl.begin_I2C();

  #if SERIAL_DEBUG_PRINTS_ON
  if (!lsm6ds_success){
    Serial.println("Failed to find LSM6DS chip");
  }
  if (!lis3mdl_success){
    Serial.println("Failed to find LIS3MDL chip");
  }
  
  Serial.println("LSM6DS and LIS3MDL Found!");
  #endif
  
  /*******************************************************************************/
  /* Configure accelerometer range */
  lsm6ds.setAccelRange(LSM6DS_ACCEL_RANGE_2_G);

  /* Configure accelerometer data rate */
  lsm6ds.setAccelDataRate(LSM6DS_RATE_208_HZ);

  /* Configure gyro range */
  lsm6ds.setGyroRange(LSM6DS_GYRO_RANGE_250_DPS);

  /* Configure gyro data rate */
  lsm6ds.setGyroDataRate(LSM6DS_RATE_208_HZ);

  /* Configure magnetometer data rate */
  lis3mdl.setDataRate(LIS3MDL_DATARATE_155_HZ);
  // You can check the datarate by looking at the frequency of the DRDY pin

  /* Configure magnetometer data range */
  lis3mdl.setRange(LIS3MDL_RANGE_4_GAUSS);

  /* Configure magnetometer performance mode */
  lis3mdl.setPerformanceMode(LIS3MDL_HIGHMODE);

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

  accelX = accel.acceleration.x; accelY = accel.acceleration.y; accelZ = accel.acceleration.z;
  gyroX = gyro.gyro.x; gyroY = gyro.gyro.y; gyroZ = gyro.gyro.z;
  magX = mag.magnetic.x; magY = mag.magnetic.y; magZ = mag.magnetic.z;
  
  /**********************************************************************************************/
  /********************************[BEGIN CAN-BUS PROTOCOL LOOP]**********************************/
  /**********************************************************************************************/
  if(CAN_Send_Delay.justFinished()) // Checking the delay on each loops
  {
    // Delay has expired so set it to run again. 
    // This library does not allow drift and logs the actual expiration time. 
    CAN_Send_Delay.repeat(); 

    /**********************************************************************************************/
//    /* DEBUG PRINT LINES */
    #if SERIAL_DEBUG_PRINTS_ON
    // Acceleration data
    Serial.print(accelX);Serial.print(","); Serial.print(accelY);Serial.print(","); Serial.print(accelZ);Serial.print(",");
    // Gyroscope data
    Serial.print(gyroX);Serial.print(",");
    Serial.print(gyroY);Serial.print(",");
    Serial.print(gyroZ);Serial.print(",");
    // Magnetometer data
    Serial.print(magX);Serial.print(",");
    Serial.print(magY);Serial.print(",");
    Serial.println(magZ);
    #endif
    /**********************************************************************************************/
  
    /* Encoding each value into the 64 bit can message per the 
    // bit structure, scaling and offset defined in the .dbc file
    // this puts the data into storage in the can_obj variable.
    // its datatype was created by dbcc and encompaseses the whole .dbc
    */
    // ACCELERATION DATA
    encode_can_0x102_Raw_Accel_x_mps2(&can_obj,accelX); 
    encode_can_0x102_Raw_Accel_y_mps2(&can_obj,accelY);
    encode_can_0x102_Raw_Accel_z_mps2(&can_obj,accelZ);

    // GYROSCOPE DATA
    encode_can_0x102_Raw_Gyro_x_rps(&can_obj,gyroX); 
    encode_can_0x102_Raw_Gyro_y_rps(&can_obj,gyroY);
    encode_can_0x102_Raw_Gyro_z_rps(&can_obj,gyroZ);

    // MAGNETOMETER DATA
    encode_can_0x103_Raw_Mag_x_uT(&can_obj,magX); 
    encode_can_0x103_Raw_Mag_y_uT(&can_obj,magY);
    encode_can_0x103_Raw_Mag_z_uT(&can_obj,magZ);
    
    
    // This is the union datatype that allows us to read in uint64_t or uint8_t[8]
    MsgData msg_102;
    MsgData msg_103;

    // This command unloads the message
    pack_message(&can_obj,0x102,&msg_102.a); // Gyro & accel
    pack_message(&can_obj,0x103,&msg_103.a); // Mag

    // Accel & Gyro
    CAN.beginPacket(0x102,8,false);
    CAN.write(msg_102.b,8);
    CAN.endPacket();  

    // Mag
    CAN.beginPacket(0x103,8,false);
    CAN.write(msg_103.b,8);
    CAN.endPacket();  
  }    
  
}
