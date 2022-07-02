#include "led_driver.hpp"

LedDriver::LedDriver(int _gpio)
	: gpio(_gpio)
{
	gpioSetMode(gpio, PI_OUTPUT);
}

void LedDriver::countdown(float spacing_s, float pulsewidth_s, int num_pulses)
{
	for (int i = 0; i < num_pulses; ++i) {
		gpioWrite(gpio, 1);
		std::this_thread::sleep_for(
			std::chrono::milliseconds(static_cast<int>(1000*pulsewidth_s))
		);
		gpioWrite(gpio, 0);
		std::this_thread::sleep_for(
			std::chrono::milliseconds(static_cast<int>(1000*spacing_s))
		);
	}
}
