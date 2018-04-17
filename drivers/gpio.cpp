#include <drivers/gpio.h>

void GPIO_Init(GPIO * this, GPIO_PORT port, GPIO_PIN pin, GPIO_DIRECTION direction)
{
	return;
}

GPIO_ETAT GPIO_Read(GPIO * this)
{
	return LOW;
}

void GPIO_Write(GPIO * this, GPIO_ETAT etat)
{
	return;
}

void GPIO_Toggle(GPIO * this)
{
	return;
}