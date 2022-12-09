// https://github.com/lcdwiki/LCDWIKI_gui/blob/master/Document/LCDWIKI%20GUI%20lib%20Manual.pdf

#include <Wire.h>
#include <millisDelay.h>

#include <LCDWIKI_GUI.h> //Core graphics library
#include <LCDWIKI_KBV.h> //Hardware-specific library
LCDWIKI_KBV mylcd(ILI9341,A3,A2,A1,A0,A4);

//define some colour values
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

// Variables
const int msgSize = 4;
byte msgRead[msgSize]; // Message in bytes (8 maximum)
uint16_t RPM_1, RPM_2;
int i;

millisDelay Delay; 
void setup()
{
  Wire.begin();
  Serial.begin(115200);
  mylcd.Init_LCD();
  mylcd.Fill_Screen(BLACK);
  mylcd.Set_Text_Mode(2);
  mylcd.Set_Text_colour(GREEN);
  mylcd.Set_Text_Back_colour(BLACK);
  mylcd.Set_Text_Size(2);
  Delay.start(100);
}

void loop()
{
  /****************************************************************/
  // I2C
  /****************************************************************/
  Wire.requestFrom(54,msgSize);

  for (i = 0; Wire.available(); i++) {
        msgRead[i] = Wire.read();
    }

  read_msg(RPM_1, msgRead, 0);
  read_msg(RPM_2, msgRead, 2);

  Serial.print(RPM_1); Serial.print(" "); Serial.println(RPM_2);

  if(Delay.justFinished()) // Checking the delay on each loops
  {
    // Delay has expired so set it to run again. 
    // This library does not allow drift and logs the actual expiration time. 
    Delay.repeat(); 
    mylcd.Fill_Rect(30, 30, 100, 50, RED);
    mylcd.Print_Number_Int(RPM_2, 30, 30, 5, ' ',10);
  }
}

void read_msg(uint16_t &x, byte *Arr, int start_bit) {
  x = Arr[start_bit];
  x = (x << 8) | Arr[++start_bit];
}
