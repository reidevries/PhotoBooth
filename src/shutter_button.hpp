#ifndef __SHUTTER_BUTTON_HPP_
#define __SHUTTER_BUTTON_HPP_

#include <pigpio.h>
#include <iostream>

class ShutterButton
{
	int gpio;
	re_decoderCB_t callback;
public:
	ShutterButton(int _gpio, re_decoderCB_t _callback);
	~ShutterButton();
};

#endif
