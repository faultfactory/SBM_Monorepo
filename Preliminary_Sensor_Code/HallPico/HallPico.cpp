#include "setup.hpp"
int main() {
	stdio_init_all();
	cout << "Start" << endl;
	cout << "Waiting for " << I2C_INIT_DELAY << "ms before I2C init" << endl;
	sleep_ms(I2C_INIT_DELAY);

	// i2cInit();
	// i2cPrint();

	if (cyw43_arch_init()) {
		printf("WiFi init failed");
		return -1;
	}

	multicore_reset_core1();
	multicore_launch_core1(core1);
	while (true) {
		loop();
	}

	return 0;
}

void loop() {
	// auto test = i2c_get_read_available(I2C_PORT);
	// if (test != 0){
	// 	cout << test << endl;
	// }
	sendRPMs();
	sleep_ms(1000);
}

void loop1() {
	
}

void hallTrig(uint gpio, uint32_t events) {
	// cout << gpio << endl;
	uint8_t hall = -1;
	for (uint8_t i = 0; i < numHalls; i++) {
		if (halls[i] == gpio) {
			hall = i;
			break;
		}
	}
	if (hall != -1) {
		uint64_t t = time_us_64();
		uint64_t dt = t - prevTimes[hall];
		dt = 3000; // TODO remove
		if (dt > debounceTime) {
			timeDifs[hall] = dt;
			prevTimes[hall] = t;
			#ifdef VERBOSE
			cout << "dt: " << dt << endl;
			cout << "rpm: " << rpm(dt) << endl;
			cout << "mph: " << mph(dt) << endl;
			#endif
			toggleLed();
		}
	} else {
		printf("ERROR: Hall pins may have been set up incorrectly.\n");
	}
}

void sendRPMs(){
	uint16_t rpms[numHalls];
	for (int i = 0; i < numHalls; i++){
		if (timeDifs[i]){
			rpms[i] = rpm(timeDifs[i]);
		} else {
			rpms[i] = 0;
		}
		cout << timeDifs[i] << " ";
	}
	uint8_t r8[numHalls*2];
	for (int i = 0; i < numHalls; i++){
		uint16_t r = rpms[i];
		uint8_t part1 = r;
		uint8_t part2 = r>>8;
		r8[2*i] = part1;
		r8[2*i+1] = part2;
	}
	i2c_write_raw_blocking(I2C_PORT, r8, numHalls*2);
	cout << "written" << endl;
}