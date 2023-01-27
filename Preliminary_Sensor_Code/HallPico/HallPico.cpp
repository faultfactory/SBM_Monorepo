#include "setup.hpp"
int main() {
	stdio_init_all();
	cout << "Start" << endl;
	cout << "Waiting for " << I2C_INIT_DELAY << "ms before I2C init" << endl;
	sleep_ms(I2C_INIT_DELAY);

	i2cInit();

	if (cyw43_arch_init()) {
		printf("WiFi init failed");
		return -1;
	}

	setLed(1);

	multicore_reset_core1(); // Likely not required to be done since multicore launch happens basically at bootup, but just in case.
	multicore_launch_core1(setup1);

	while (true) {
		loop();
	}

	return 0;
}

void loop() {
	sendRPMs();
	sleep_ms(DATA_INTERVAL);
}

void loop1() {
	#ifdef TEST_MODE
	for (int i = 0; i < NUM_HALLS; i++) {
		if (states[i]) {
			timeDifs[i]--;
		} else {
			timeDifs[i]++;
		}
		if (timeDifs[i] > TEST_MAX || timeDifs[i] < TEST_MIN) {
			states[i] = !states[i];
		}
	}
	sleep_ms(DATA_INTERVAL);
	#endif
}

void hallTrig(uint gpio, uint32_t events) {
	uint64_t t = time_us_64();
	#ifdef VERBOSE
	cout << gpio << endl;
	#endif
	uint8_t hall = -1;
	for (uint8_t i = 0; i < NUM_HALLS; i++) {
		if (HALLS[i] == gpio) {
			hall = i;
			break;
		}
	}
	if (hall != -1) {
		uint64_t dt = t - prevTimes[hall];
		if (dt > DEBOUNCE_TIME) {
			timeDifs[hall] = dt;
			prevTimes[hall] = t;
			#ifdef VERBOSE
			cout << "dt: " << dt << endl;
			cout << "rpm: " << rpm(dt) << endl;
			cout << "mph: " << mph(dt) << endl;
			#endif
			// toggleLed();
		}
	} else {
		printf("ERROR: Hall pins may have been set up incorrectly.\n");
	}
}

void sendRPMs() {
	for (int i = 0; i < NUM_HALLS; i++) {
		if (timeDifs[i]) {
			rpms[i] = rpm(timeDifs[i]);
		} else {
			rpms[i] = 0;
		}
		#ifdef VERBOSE
		cout << timeDifs[i] << " ";
		#endif
	}
	uint8_t r8[NUM_HALLS * 2]; // i2c needs sent data as single bytes, so spliting 1 uint16_t into 2 uint8_t
	for (int i = 0; i < NUM_HALLS; i++) {
		uint16_t r = rpms[i];
		uint8_t part1 = r;
		uint8_t part2 = r >> 8;
		r8[2 * i] = part1;
		r8[2 * i + 1] = part2;
	}
	i2c_write_raw_blocking(I2C_PORT, r8, NUM_HALLS * 2);
	#ifdef VERBOSE
	cout << "written" << endl;
	#endif
}