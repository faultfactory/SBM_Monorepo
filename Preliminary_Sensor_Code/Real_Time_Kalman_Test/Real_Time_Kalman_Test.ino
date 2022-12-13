/* CAN-Bus transmit vehicle dynamics node*/

#include <CAN.h>
#include <millisDelay.h>
#include <Adafruit_LSM6DSOX.h>
#include <Adafruit_LIS3MDL.h>
#include <BasicLinearAlgebra.h>
#define SERIAL_DEBUG_PRINTS_ON 0

using namespace BLA;
/*******************************************************************************/

// DEBUG VARIABLES
// input variables
const float g = 9.81;

// loop variables
float Tout, N, Tp;

// State
BLA::Matrix<2,1> Z;
BLA::Matrix<2,1> f;

// Sensor Models (Update)
BLA::Matrix<3,1> y_accel;
BLA::Matrix<2,2> A;
BLA::Matrix<2,2> Ad;
BLA::Matrix<3,2> C_accel;
BLA::Matrix<2,3> L_accel;

// Covariance
BLA::Matrix<2,2> P; 
BLA::Matrix<2,2> Q;
// Sensor Covariance
BLA::Matrix<3,3> R_accel; 

// Sensor Data Package
BLA::Matrix<3,1> accel_Raw;

// Misc
BLA::Matrix<2,2> I;
BLA::Matrix<3,3> temp;

/*******************************************************************************/
// Create IMU objects
Adafruit_LSM6DSOX lsm6ds;
Adafruit_LIS3MDL lis3mdl;
bool lsm6ds_success, lis3mdl_success;
float accelX, accelY, accelZ, gyroX, gyroY, gyroZ, magX, magY, magZ;
float p, q, r, mbx, mby, mbz, Vg;
float aLPF = 0.0;

int SCALE = 1000000;

float vale = 0.0001;

millisDelay CAN_Send_Delay, Predict_Delay; 
void setup()
{
  /*******************************************************************************/
  // Fill Diag Covariance Matrices
  Z.Fill(0); P.Fill(0); Q.Fill(0); R_accel.Fill(0); accel_Raw.Fill(0);
  for (int i = 0; i < P.Rows; ++i) {
    P(i,i) = 1.0; Q(i,i) = 1.0;
  }

  Q = {1.0,0.0,0.0,1.0};
  R_accel = {1.0,0,0,0,1.0,0,0,0,1.0};
    
  // Initialize Raw Values
  p = 0.0; q = 0.0; r = 0.0; Vg = 0.0; mbx = 0.0; mby = 0.0; mbz = 0.0;

  // Additional Info
  Tout = 5.0/1000.0; // Time in seconds
  N = 2.0; // Number of iterations?
  Tp = Tout/N;

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
  Predict_Delay.start(Tout*1000);
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
  lsm6ds.setAccelRange(LSM6DS_ACCEL_RANGE_4_G);

  /* Configure accelerometer data rate */
  lsm6ds.setAccelDataRate(LSM6DS_RATE_104_HZ);

  /* Configure gyro range */
  lsm6ds.setGyroRange(LSM6DS_GYRO_RANGE_250_DPS);

  /* Configure gyro data rate */
  lsm6ds.setGyroDataRate(LSM6DS_RATE_104_HZ);

  /* Configure magnetometer data rate */
  lis3mdl.setDataRate(LIS3MDL_DATARATE_80_HZ);
  // You can check the datarate by looking at the frequency of the DRDY pin

  /* Configure magnetometer data range */
  lis3mdl.setRange(LIS3MDL_RANGE_8_GAUSS);

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
  sensors_event_t accel, gyro, mag, tempv;

  lsm6ds.getEvent(&accel, &gyro, &tempv);
  lis3mdl.getEvent(&mag);

  /* LOW PASS FILTERING */
  accelX = aLPF*accelX + (1 - aLPF)*accel.acceleration.x; accelY = aLPF*accelY + (1 - aLPF)*accel.acceleration.y; accelZ = aLPF*accelZ + (1 - aLPF)*accel.acceleration.z;
  gyroX = aLPF*gyroX + (1 - aLPF)*gyro.gyro.x; gyroY = aLPF*gyroY + (1 - aLPF)*gyro.gyro.y; gyroZ = aLPF*gyroZ + (1 - aLPF)*gyro.gyro.z;
  magX = aLPF*magX + (1 - aLPF)*mag.magnetic.x; magY = aLPF*magY + (1 - aLPF)*mag.magnetic.y; magZ = aLPF*magZ + (1 - aLPF)*mag.magnetic.z;

  /**********************************************************************************************/
  /**********************************[Extended Kalman Filter]************************************/
  /**********************************************************************************************/
  
  if(CAN_Send_Delay.justFinished()) // Checking the delay on each loops
  {
    CAN_Send_Delay.repeat(); 
    if (accel_Raw(1) == accelX || p == gyroX) {
      for (int i = 1; i <= N; ++i)  {    // I don't remember what pre-incrementing vs post-incrementing does for a for loop but that might be something to look at. 
        if(Predict_Delay.justFinished()) {
          Predict_Delay.repeat();
          Serial.println("Predict");
    
          /* PREDICTION STEP */
          f = {p + r*cos(Z(0))*tan(Z(1)) + q*sin(Z(0))*tan(Z(1)),
                          q*cos(Z(0)) - r*sin(Z(0))};
          
          Z = Z + f*(Tout/N); // BUG_MAYBE: Integer division will drive this term to zero if Tout < N. Looks like it's already a float. Maybe cast as float. 
          // Z = Z + f*(Tout/(float)N)

          /* Reccomend you pre-compute some of these within a scope for speed for this large calculation. Compiler may do that automatically but it would require less 
          parenthesis and pow(blah,2) is not optimized. Also explicitly call a float a float. Who knows what's going on behind the scenes.
          Double check my math if you use this. Might not be necessary at all but worth looking

          { 
            float cos_z0 = cos(Z(0));
            float tan_z1 = tan(Z(1));
            float sin_z0 = sin(Z(0));

            A = {q*cos_z0*tan_z1 - r*sin_z0*tan_z1, r*cos_z0*(tan_z1*tan_z1 + 1f) + q*sin_z0*(tan_z1*tan_z1 + 1f),
                     - r*cos_z0 - q*sin_z0,                                                             0f};

          */
          A = {q*cos(Z(0))*tan(Z(1)) - r*sin(Z(0))*tan(Z(1)), r*cos(Z(0))*(pow(tan(Z(1)),2.0) + 1) + q*sin(Z(0))*(pow(tan(Z(1)),2.0) + 1),
                    - r*cos(Z(0)) - q*sin(Z(0)),                                                             0};
          Ad = I + A*Tp + (A*A)*pow(Tp,2.0);
          P = Ad*P*~Ad + Q*pow(Tp,2.0);
        }
      }
    }
      /*GYROSCOPE UPDATE*/
      Serial.println("Gyro Update");
      p = gyroX; q = gyroY; r = gyroZ;
      
      /*ACCELEROMETER UPDATE*/
      Serial.println("Accel Update");
    
      accel_Raw = {accelX,accelY,-accelZ};
      C_accel = {                     0,           g*cos(Z(1)),
                -g*cos(Z(0))*cos(Z(1)), g*sin(Z(0))*sin(Z(1)),
                 g*cos(Z(1))*sin(Z(0)), g*cos(Z(0))*sin(Z(1))};
      
      temp = R_accel + C_accel*P*~C_accel; Invert(temp); // Compute inverse
      L_accel = P*~C_accel*temp;
      P = (I - L_accel*C_accel)*P*~(I - L_accel*C_accel) + L_accel*R_accel*~L_accel;
      // Accelerometer Model
      y_accel = {g*sin(Z(1)),
              -g*cos(Z(1))*sin(Z(0)),
              -g*cos(Z(0))*cos(Z(1))};
      Z = Z + L_accel*(accel_Raw - y_accel);

//      for (int i = 0; i < 2; ++i) {
//        for (int j = 0; j < 2; ++j) {
//          Serial.print((P)(i,j)); Serial.print(" ");
//        } Serial.println(" ");
//      }
      
    Serial.print(Z(0)*(180.0/PI)); Serial.print(" "); Serial.println(Z(1)*(180.0/PI));
  }  
}
