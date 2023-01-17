#include <Wire.h>

// Variables
const int msgSize = 4;
byte msgRead[msgSize]; // Message in bytes (8 maximum)
uint16_t RPM_1, RPM_2;
int i;

void setup() {
	Wire.begin();
	Serial.begin(115200);
	Serial.println("Begin");
}

void loop() {
	Wire.requestFrom(8, msgSize);

	for (i = 0; Wire.available(); i++) {
		Serial.println("available");
		Serial.println(Wire.read());

		// msgRead[i] = Wire.read();
	}

	// read_msg(RPM_1, msgRead, 0);
	// read_msg(RPM_2, msgRead, 2);

	// Serial.print(RPM_1); Serial.print(" "); Serial.println(RPM_2);
}

void read_msg(uint16_t& x, byte* Arr, int start_bit) {
	x = Arr[start_bit];
	x = (x << 8) | Arr[++start_bit];
}



void scan() {
	byte error, address;
	int nDevices;

	Serial.println("Scanning...");

	nDevices = 0;
	for (address = 1; address < 127; address++) {
		// The i2c_scanner uses the return value of
		// the Write.endTransmisstion to see if
		// a device did acknowledge to the address.
		Wire.beginTransmission(address);
		error = Wire.endTransmission();

		if (error == 0) {
			Serial.print("I2C device found at address 0x");
			if (address < 16)
				Serial.print("0");
			Serial.print(address, HEX);
			Serial.println("  !");

			nDevices++;
		} else if (error == 4) {
			Serial.print("Unknown error at address 0x");
			if (address < 16)
				Serial.print("0");
			Serial.println(address, HEX);
		}
	}
	if (nDevices == 0)
		Serial.println("No I2C devices found\n");
	else
		Serial.println("done\n");

	delay(1000);
}