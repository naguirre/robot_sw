#include "drivers/gpio.h"

Gpio::Gpio(Gpio::Port port, Gpio::Pin pin, Gpio::Direction direction)
{
    this->port = port;
    this->pin  = pin;
    this->direction = direction;
    this->state = Gpio::LOW;
}

Gpio::State Gpio::Read()
{
	return this->state;
}

void Gpio::Write(Gpio::State state)
{
	this->state = state;
}

void Gpio::Toggle(void)
{
	if (this->state == Gpio::LOW)
    {
        this->state = Gpio::HIGH;
    }
    else
    {
        this->state = Gpio::LOW;
    }
}
