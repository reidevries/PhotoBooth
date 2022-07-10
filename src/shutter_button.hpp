#ifndef __SHUTTER_BUTTON_HPP_
#define __SHUTTER_BUTTON_HPP_

#include <pigpio.h>
#include <iostream>

class ShutterButton
{
public:
	class Callback
	{
	public:
		virtual void button_pressed(uint32_t tick) = 0;
		virtual void button_released(uint32_t tick) = 0;
	};

private:
	int gpio;
	Callback *callback;
	static void pressed(
		int _gpio,
		int level,
		uint32_t tick,
		void *callback
	); 

public:
	ShutterButton(int _gpio, Callback *callback);
	void cancel();
};

#endif
