#ifndef __SHUTTER_BUTTON_HPP_
#define __SHUTTER_BUTTON_HPP_

#include <pigpio.h>
#include <iostream>

class ShutterButton
{
	int gpio;
	void (*callback)();
	static void pressed(int _gpio, int level, uint32_t tick, void *callback); 

public:
	ShutterButton(int _gpio, void (*_callback)());
	~ShutterButton();
};

#endif
