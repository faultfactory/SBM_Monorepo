#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "pico/multicore.h"
#include "pico/cyw43_arch.h"
#include <iostream>
using namespace std;
#define TEETH 32.0
// #define VERBOSE

const uint8_t numHalls = 2;
const uint8_t halls[numHalls] = { 2,3 };
uint64_t prevTimes[numHalls];
uint64_t timeDifs[numHalls];
uint16_t rpms[numHalls];
// const uint64_t debounceTime = 2500; // minimum time between switching events to stop it from having near infinite speed.
const uint64_t debounceTime = 100;

void loop();
void loop1();
void hallTrig(uint gpio, uint32_t events);

uint64_t rpm(uint64_t dt);
double rpmD(uint64_t dt);
double rpm2mph(double rpm);
double mph(uint64_t dt);
uint64_t calcSpeed(uint64_t dt);

void core1();

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define I2C_ADDR 0x8
#define I2C_INIT_DELAY 0
#define DATA_INTERVAL 100

void i2cInit();
bool reserved_addr(uint8_t addr);
void i2cPrint();
void sendRPMs();

bool led = false;
void setLed();
void setLed(bool status);
void toggleLed();
void blink();

uint64_t rpm(uint64_t dt) {
	return (60e6) / (TEETH * dt);
}
double rpmD(uint64_t dt) {
	return (60e6) / (TEETH * dt);
}
double rpm2mph(double rpm) {
	return (rpm * 72 * 3.14 * 0.27) / (965.6064);
}
double mph(uint64_t dt) {
	double r = rpm(dt);
	return rpm2mph(r);
}
uint64_t calcSpeed(uint64_t dt) {
	return uint64_t(60.0 / (TEETH * ((double)((dt) / 1000000.0))));
}

void core1() {
	gpio_set_irq_enabled_with_callback(2, GPIO_IRQ_EDGE_RISE, true, &hallTrig);
	gpio_set_irq_enabled_with_callback(3, GPIO_IRQ_EDGE_RISE, true, &hallTrig);
	while (true) {
		loop1();
	}
}

void i2cInit() {
	i2c_init(I2C_PORT, 10000);
	i2c_set_slave_mode(I2C_PORT, true, I2C_ADDR);
	gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
	gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
	gpio_pull_up(I2C_SDA);
	gpio_pull_up(I2C_SCL);
	// bi_decl(bi_2pins_with_func(I2C_SDA, I2C_SCL, GPIO_FUNC_I2C));
}
bool reserved_addr(uint8_t addr) {
	return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
}
void i2cPrint() {
	printf("\nI2C Bus Scan\n");
	printf("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");

	for (int addr = 0; addr < (1 << 7); ++addr) {
		if (addr % 16 == 0) {
			printf("%02x ", addr);
		}

		// Perform a 1-byte dummy read from the probe address. If a slave
		// acknowledges this address, the function returns the number of bytes
		// transferred. If the address byte is ignored, the function returns
		// -1.

		// Skip over any reserved addresses.
		int ret;
		uint8_t rxdata;
		if (reserved_addr(addr)) {
			ret = PICO_ERROR_GENERIC;
		} else {
			ret = i2c_read_blocking(I2C_PORT, addr, &rxdata, 1, false);
		}

		printf(ret < 0 ? "." : "@");
		printf(addr % 16 == 15 ? "\n" : "  ");
	}
	printf("Done.\n");
}

void setLed() {
	cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led);
}
void setLed(bool status) {
	cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, status);
}
void toggleLed() {
	led = !led;
	// setLed();
}
void blink() {
	toggleLed();
	sleep_ms(100);
}
