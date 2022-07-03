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
	
public:
	void init(int gpio);

	void countdown(float spacing_s, float pulsewidth_s, int num_pulses);
};

#endif
