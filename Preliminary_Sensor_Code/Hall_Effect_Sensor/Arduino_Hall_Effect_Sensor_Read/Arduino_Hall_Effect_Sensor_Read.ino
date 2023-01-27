#include <Wire.h>
#include <CAN.h>
#include <millisDelay.h>
#include "can_conv/sbm_network_definition.h"
#define SERIAL_DEBUG_PRINTS_ON 1

millisDelay CAN_Send_Delay;

// CAN bus
/* This defines the data storage object that the generated code us//s */
static can_obj_sbm_network_definition_h_t can_obj;

// Unions are special datatypes that provide two views of the same memmory location.
union MsgData {
	uint64_t a;   // dbcc output uses uint64_t
	uint8_t b[8]; // Adafruit CAN library wants uint8_t[8]
};

const int msgSize = 4;
uint8_t msgRead[msgSize]; // Message in bytes (8 maximum)
uint16_t RPM_1, RPM_2;

void setup() {
	Wire.begin();
	Serial.begin(115200);
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, HIGH);

	// Setup M4 CAN board
	pinMode(PIN_CAN_STANDBY, OUTPUT);
	digitalWrite(PIN_CAN_STANDBY, false); // turn off STANDBY
	pinMode(PIN_CAN_BOOSTEN, OUTPUT);
	digitalWrite(PIN_CAN_BOOSTEN, true); // turn on booster

	Serial.println("Begin");

	CAN.begin(500000);
	CAN_Send_Delay.start(10); //10 ms -> 100 Hz (Sending 2 messages at 100 Hz, so 200 messages/sec)
}

void loop() {

	// scan();
	if (CAN_Send_Delay.justFinished()) // Checking the delay on each loops
	{
		// Delay has expired so set it to run again. 
		// This library does not allow drift and logs the actual expiration time. 
		CAN_Send_Delay.repeat();

		Wire.requestFrom(8, 4);

		for (int i = 0; Wire.available(); i++) {
			msgRead[i] = Wire.read();
		}

		RPM_1 = msgRead[1] * (1 << 8) + msgRead[0]; // Combining 2 uint8_t into 1 uint16_t
		RPM_2 = msgRead[3] * (1 << 8) + msgRead[2];
		// Serial.print(RPM_1); Serial.print(" "); Serial.println(RPM_2);

		// WHEEL SPEED
		encode_can_0x098_FL_Wheelspeed_rpm(&can_obj, RPM_1);
		encode_can_0x098_FR_Wheelspeed_rpm(&can_obj, RPM_2);

		MsgData msg_098;
		pack_message(&can_obj, 0x098, &msg_098.a);
		CAN.beginPacket(0x098, 8, false);
		CAN.write(msg_098.b, 8);
		CAN.endPacket();
	}
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
	if (nDevices == 0) {
		Serial.println("No I2C devices found\n");
	} else {
		Serial.println("done\n");
	}

	delay(1000);
}
