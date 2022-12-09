/* This program reads & calibrates the pressure transducers used in this experiement
 *  For use on an Arduino Mega 2560
 *  
 * Pressure transducer type: MIPAN2XX500PSAAX
 *  MIP: Media isolated pressure
 *  A: Electrical connector type
 *  N2: Pressure port type, 1/8-27 NPT
 *  XX: Special
 *  500P: Pressure range of up to 500 psi
 *  S: Sealed gauge
 *  AA: Output transfer function
 *  X: Special
 */

//Arduino variables
const int analogInput1 = A0; //Analog input pin for pressure transducer #1 [INPUT]
const int analogInput2 = A1; //Analog input pin for pressure transducer #2 [OUTPUT]

const int resolution = 1024; //The arduino's analog voltage reading resolution in total units
const int baudRate = 115200; //Constant integer to set the baud rate for serial monitor

//Transducer variables
const int numTrans = 2; //Number of transducers used
const float Vsupply = 5.0; //(V), Voltage supplied
const int Pmax = 500; //(psi), The maximum pressure the transducer can produce
const int Pmin = 0; //(psi), The minimum pressure the transducer can produce

//Reading & Calibration variables
int voltageRead = 0; //Variable that stores the value read
float voltageReal[numTrans]; //Actual voltage being read
float Vmin[numTrans]; //Minimum voltage needed to achieve Pmin (0 psi at STP)
float Vmax = 0.9*Vsupply; //Maximum voltage needed to achieve Pmax (500 psi at STP)
float Papplied[numTrans]; //Calcualted pressure applied
float dataArr[numTrans];

//Data averaging
const int sample_sz = 50;
float runAvg[numTrans][sample_sz];
int i = 0;
int j = 0;
int num = 0;
int ctr = 0;
long fullAvg[numTrans];
int voltageAvg[numTrans];

//csv file creation variables
float curr[numTrans];
float threshold = 0.65; //psi

void setup() 
{
  Serial.begin(baudRate); //Initializes serial communication at set baud rate bits per second

  Vmin[0] = 0.0947*Vsupply; //Calibration for transducer #1
  Vmin[1] = 0.0957*Vsupply; //Calibration for transducer #2

  for(i = 0; i < numTrans; i++) {
      fullAvg[num] = 0;
      voltageAvg[num] = 0;
      voltageReal[num] = 0.0;
      
    for(j = 0; j < sample_sz; j++){
      runAvg[i][j] = 0;
    }
  }

  for(i = 0; i < numTrans; i++){
    Papplied[i] = 0.0;
  }
}

void loop() 
{  
  voltageRead = analogRead(analogInput1);
  dataArr[0] = voltageRead;

  voltageRead = analogRead(analogInput2);
  dataArr[1] = voltageRead;

  
  // Take average of pressure samples
  for(num = 0; num < numTrans; num++) {
    runAvg[num][ctr] = dataArr[num]; 
  }
  ctr++;
  if(ctr >= sample_sz){
    ctr = 0;
    for(num = 0; num < numTrans; num++) {
      for(i = 0; i < sample_sz; i++){
        fullAvg[num] = fullAvg[num] + runAvg[num][i];
      }
    }

    for(num = 0; num < numTrans; num++) {
      voltageAvg[num] = fullAvg[num]/sample_sz;
  
      voltageReal[num] = (Vsupply/resolution)*voltageAvg[num];
    
      // Transfer function for ratiometric output ranging from 10% Vsupply at null pressure to 90% Vsupply at full scale pressure
      Papplied[num] = ((Pmax-Pmin)/(Vmax-Vmin[num]))*(voltageReal[num] - Vmin[num]) + Pmin;

    }
  }

  Serial.print("Press 1: "); Serial.print(Papplied[0]); Serial.print("\n");
  
  for(num = 0; num < numTrans; num++) {
    fullAvg[num] = 0;
  }
}
