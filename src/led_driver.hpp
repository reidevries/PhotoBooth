#ifndef __LED_DRIVER_HPP_
#define __LED_DRIVER_HPP_

#include <pigpio.h>
#include <iostream>
#include <chrono>
#include <thread>



class LedDriver
{
private:
	int gpio;

	struct PulseArgs
	{
		int gpio;
		float spacing_s;
		float pulsewidth_s;
	};
	static void* pulse(void *arg);
public:
	void init(int gpio);

	void countdown(float spacing_s, float pulsewidth_s, int num_pulses);

	pthread_t* start_pulse(float spacing_s, float pulsewidth_s);
	void stop_pulse(pthread_t* thread_ptr);
};

#endif
