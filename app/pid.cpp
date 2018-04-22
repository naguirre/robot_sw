#include <app/pid.h>

Pid::Pid(float period, float kp, float ki, float setPoint, float maxCommand)
{
    this->settings.period     = period;
    this->settings.maxCommand = maxCommand;
    this->settings.kp         = kp;
    this->settings.ki         = ki;

    this->ComputeDiscrete();

    this->inputs.setPoint = setPoint;
}

void Pid::ComputeDiscrete(void)
{
    this->settings.discrete.num[0] = this->settings.ki * this->settings.period / 2 - this->settings.kp;
    this->settings.discrete.num[1] = this->settings.ki * this->settings.period / 2 + this->settings.kp;
    this->settings.discrete.den[0] = -1.0;
    this->settings.discrete.den[1] = 1.0;
}

void Pid::ComputeCommand(void)
{
    //Calcul de l'error entre l'this->state et la setPoint
    this->state.error = this->inputs.setPoint - this->inputs.measure;

    this->state.command = (float)(this->settings.discrete.num[1] * this->state.error
                                + this->settings.discrete.num[0] * this->state.lastError
                                - this->settings.discrete.den[0] * this->state.lastCommand)
                                / this->settings.discrete.den[1];

    this->state.lastCommand = this->state.command;
    this->state.command = Maths::Limit(this->state.command, this->settings.maxCommand);
    this->output = this->state.command / this->settings.maxCommand;

    this->state.lastError = this->state.error;
}

float Pid::Run(float setPoint, float measure)
{
    this->inputs.setPoint = setPoint;
    this->inputs.measure  = measure;

    ComputeCommand();

    return this->output;
}

void Pid::SetKp(float kp)
{
    this->settings.kp = kp;
    ComputeDiscrete();
}

void Pid::SetKi(float ki)
{
    this->settings.ki = ki;
    ComputeDiscrete();
}

float Pid::GetKp()
{
    return this->settings.kp;
}

float Pid::GetKi()
{
    return this->settings.ki;
}

float Pid::GetSetPoint()
{
    return this->inputs.setPoint;
}
