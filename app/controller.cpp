#include <app/controller.h>
#include <maths/maths.h>
#include <iostream>

Controller::Controller(float period, float maxSpeed, float maxAcceleration) :
    period(period),
    maxSpeed(maxSpeed),
    maxAcceleration(maxAcceleration)
{
}

Controller::~Controller(void)
{
}

void Controller::SetMaxSpeed(float maxSpeed)
{
    this->maxSpeed = maxSpeed;
}

void Controller::DefineSetPoint(float value)
{
    this->setPoint = value;
}

float Controller::GetSetPoint()
{
    return this->setPoint;
}
