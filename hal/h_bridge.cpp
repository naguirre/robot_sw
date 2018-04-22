#include <hal/h_bridge.h>

HBridge::HBridge(uint8_t idMotor)
{
    this->pwm  = new Pwm(PWM_CHANNELS[idMotor]);
    this->gpio = new Gpio(SENS_PORTS[idMotor], SENS_PINS[idMotor], Gpio::OUTPUT);

    this->pwm->SetDutyCycle(0.0);
    this->gpio->Write(Gpio::LOW);
}

HBridge::~HBridge(void)
{
    delete this->pwm;
    delete this->gpio;
}

void HBridge::SetCommand(float command)
{
    this->pwm->SetDutyCycle(Maths::Abs(command));
    this->gpio->Write((Gpio::State)Maths::IsPositive(command));
}
