#include "shutter_button.hpp"

ShutterButton::ShutterButton(int _gpio, re_decoderCB_t _callback)
	: gpio(_gpio)
	, callback(_callback)
{
	gpioSetMode(gpio, PI_INPUT);
	gpioSetPullUpDown(gpio, PI_PUD_UP);
	gpioSetAlertFuncEx(gpio, callback, this);
}

ShutterButton::~ShutterButton()
{
	gpioSetAlertFuncEx(gpio, 0, this);
}
