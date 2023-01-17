#include <Wire.h>

const int msgSize = 4;
uint8_t msgRead[msgSize]; // Message in bytes (8 maximum)
uint16_t RPM_1, RPM_2;

void setup() {
	Wire.begin();
	Serial.begin(115200);
	Serial.println("Begin");
}

void loop() {
	Wire.requestFrom(8, 4);
	
	for (int i = 0; Wire.available(); i++) {
		msgRead[i] = Wire.read();
	}

	RPM_1 = msgRead[1] * (1<<8) + msgRead[0]; // Combining 2 uint8_t into 1 uint16_t
	RPM_2 = msgRead[3] * (1<<8) + msgRead[2];
	Serial.print(RPM_1); Serial.print(" "); Serial.println(RPM_2);
}

void scan() { 
	// Taken from somewhere online, I forgot where
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