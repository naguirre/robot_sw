#include <drivers/pwm.h>

Pwm::Pwm(Pwm::Channel channel)
{
	this->channel = channel;
    this->dutyCycle = 0.0;
}

void Pwm::SetDutyCycle(float duty)
{
	this->dutyCycle = duty;
}
