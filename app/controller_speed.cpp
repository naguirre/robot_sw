#include <app/controller_speed.h>
#include <maths/maths.h>

SpeedController::SpeedController(float period, float maxSpeed, float maxAcceleration,
                                 float maxSpeedCommand, float kpSpeed, float kiSpeed) : 
    Controller(period, maxSpeed, maxAcceleration)
{
    this->speedPid    = new Pid(period, kpSpeed, kiSpeed, 0.0, maxSpeedCommand);

    this->DefineSetPoint(0.0);
}

SpeedController::~SpeedController(void)
{
    delete this->speedPid;
}

float SpeedController::Run(float speedMeasure, float positionMeasure)
{
    // Get the last speed setpoint to limit the acceleration
    float lastSetPointSpeed = this->speedPid->GetSetPoint();
    // the speed setpoint is the global controller setpoint
    float setPointSpeed = this->GetSetPoint();
    // We limit the speed setpoint and its derivate (acceleration)
    setPointSpeed = Maths::Limit(setPointSpeed, this->maxSpeed);
    setPointSpeed = Maths::LimitVariation(setPointSpeed, lastSetPointSpeed, this->period,
                                          this->maxAcceleration);
    // the mixer command is the output of speed PID
    return this->speedPid->Run(setPointSpeed, speedMeasure);
}