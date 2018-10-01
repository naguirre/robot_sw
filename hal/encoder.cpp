#include <hal/encoder.h>
#include <maths/maths.h>
#include <logs/logs.h>
#include "drivers/gpio.h"
Encoder::Encoder(uint8_t idMotor, uint16_t stepsCount, float reduction, float period)
{
    this->state.angle = 0.0;
    this->state.speed = 0.0;
    this->settings.stepsCount = stepsCount;
    this->settings.reduction  = reduction;
    this->settings.period     = period;

    this->qei = new Qei(QEI_CHANNELS[idMotor], idMotor ? (Gpio::Pin)4 : (Gpio::Pin)5);

    // Distance driven by the wheel for one encodeur step
    ComputeScaleFactor();
}

Encoder::~Encoder(void)
{
    delete this->qei;
}

void Encoder::ComputeScaleFactor(void)
{
    this->settings.scaleFactor = 2.0 * PI / (4.0 * (float)(this->settings.stepsCount) * this->settings.reduction);
}

void Encoder::Update(void)
{
    float lastAngle = this->state.angle;

    this->state.angle  = (float)qei->GetStepsCount() * this->settings.scaleFactor;
    this->state.speed  = (this->state.angle - lastAngle) / this->settings.period;
}

void Encoder::SetAngle(float value)
{
    this->state.angle = value;
    this->state.speed = 0.0;
}

float Encoder::GetAngle(void)
{
    return this->state.angle;
}

float Encoder::GetSpeed(void)
{
    return this->state.speed;
}
