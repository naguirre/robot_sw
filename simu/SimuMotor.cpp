#include <simu/SimuMotor.h>

#include <maths/maths.h>

#define SIMU_MOTOR_TIME_CONSTANT        0.2 //sec
#define SIMU_MOTOR_MAX_SPEED            50.0    //rad/sec
#define SIMU_MOTOR_VOLTAGE              12.0

#define SIMU_MOTOR_STATIC_GAIN          (SIMU_MOTOR_MAX_SPEED / SIMU_MOTOR_VOLTAGE)

SimuMotor::SimuMotor(float periodStep)
{
    this->settings.timeConstant = SIMU_MOTOR_TIME_CONSTANT;
    this->settings.staticGain   = SIMU_MOTOR_STATIC_GAIN;
    this->settings.period       = periodStep;
    this->settings.voltage      = SIMU_MOTOR_VOLTAGE;

    this->state.position  = 0.0;
    this->state.command   = 0.0;
    this->state.speed     = 0.0;
    this->state.lastSpeed = 0.0;
    this->state.block     = false;
}

void SimuMotor::SetCommand(float duty)
{
    this->state.command = this->settings.voltage * duty;
}

float SimuMotor::GetAngle()
{
    return this->state.position;
}

float SimuMotor::GetSpeed()
{
    return this->state.speed;
}

void SimuMotor::Run(void)
{
    float coeff = this->settings.timeConstant / (this->settings.timeConstant + this->settings.period);
    this->state.lastSpeed = this->state.speed;

    if (this->state.blockCount > 0)
    {
        this->state.block = true;
        this->state.blockCount--;
    }
    else
    {
        this->state.block = false;
    }

    if (this->state.block)
    {
        this->state.speed = 0.0;
    }
    else
    {
        this->state.speed = (1.0 - coeff) * this->state.command * this->settings.staticGain + coeff * this->state.lastSpeed;
    }

    this->state.position = this->state.position + this->state.speed * this->settings.period;
}
