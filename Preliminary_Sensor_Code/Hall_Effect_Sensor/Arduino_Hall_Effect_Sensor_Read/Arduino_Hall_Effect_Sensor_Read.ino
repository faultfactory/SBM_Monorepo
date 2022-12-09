#include <Wire.h>

// Variables
const int msgSize = 4;
byte msgRead[msgSize]; // Message in bytes (8 maximum)
uint16_t RPM_1, RPM_2;
int i;

void setup()
{
  Wire.begin();
  Serial.begin(115200);
}

void loop()
{
  Wire.requestFrom(54,msgSize);

  for (i = 0; Wire.available(); i++) {
        msgRead[i] = Wire.read();
    }

  read_msg(RPM_1, msgRead, 0);
  read_msg(RPM_2, msgRead, 2);

  Serial.print(RPM_1); Serial.print(" "); Serial.println(RPM_2);
}

void read_msg(uint16_t &x, byte *Arr, int start_bit) {
  x = Arr[start_bit];
  x = (x << 8) | Arr[++start_bit];
}
