#include "led_driver.hpp"

void* LedDriver::pulse(void* user_data)
{
	auto args = reinterpret_cast<PulseArgs*>(user_data);
	while (1) 
	{
		gpioWrite(args->gpio, 1);
		std::this_thread::sleep_for(std::chrono::milliseconds(
				static_cast<int>(1000*args->pulsewidth_s)
		));
		gpioWrite(args->gpio, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(
				static_cast<int>(1000*args->spacing_s)
		));
	}
}

void LedDriver::init(int gpio)
{
	this->gpio = gpio;
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

pthread_t* LedDriver::start_pulse(float spacing_s, float pulsewidth_s)
{
	auto pulse_args = new PulseArgs();
	pulse_args->gpio = gpio;
	pulse_args->spacing_s = spacing_s;
	pulse_args->pulsewidth_s = pulsewidth_s;
	return gpioStartThread(LedDriver::pulse, pulse_args);
}

void stop_pulse(pthread_t* thread_ptr)
{
	gpioStopThread(thread_ptr);
}
