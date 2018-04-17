#include "drivers/gpio.h"

GPIO::GPIO(GPIO_PORT port, GPIO_PIN pin, GPIO_DIRECTION direction)
{
	return;
}

GPIO_ETAT GPIO::Read()
{
	return LOW;
}

void GPIO::Write(GPIO_ETAT etat)
{
	return;
}

void GPIO::Toggle()
{
	return;
}
