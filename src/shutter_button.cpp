#include "shutter_button.hpp"

void ShutterButton::pressed(int gpio, int level, uint32_t tick, void *user)
{
	auto self = static_cast<ShutterButton*>(user);
	if (gpio == self->gpio) {
		if (level == 1) { // rising edge
			self->callback->button_pressed();
			std::cout << "shutter button pressed at tick " << tick << std::endl;
		}
	} else {
		std::cout << "callback called on wrong gpio number " << gpio
			<< std::endl;
	}
}

ShutterButton::ShutterButton(int _gpio, Callback *_callback)
	: gpio(_gpio)
	, callback(_callback)
{
	gpioSetMode(gpio, PI_INPUT);
	gpioSetPullUpDown(gpio, PI_PUD_UP);
	gpioSetAlertFuncEx(gpio, ShutterButton::pressed, this);
}

void ShutterButton::cancel()
{
	gpioSetAlertFuncEx(gpio, 0, this);
}
