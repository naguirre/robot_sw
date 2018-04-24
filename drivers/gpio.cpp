#include "drivers/gpio.h"

Gpio::Gpio(Gpio::Port port, Gpio::Pin pin, Gpio::Direction direction)
{
	return;
}

Gpio::State Gpio::Read()
{
	return Gpio::LOW;
}

void Gpio::Write(Gpio::State state)
{
	return;
}

void Gpio::Toggle(void)
{
	return;
}
