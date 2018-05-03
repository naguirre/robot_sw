#include <app/controller_position.h>
#include <maths/maths.h>
#include <logs/logs.h>

PositionController::PositionController(float period, float maxSpeed, float maxAcceleration,
                                       float maxSpeedCommand, float kpSpeed, float kiSpeed,
                                       float maxPositionCommand, float kpPosition, float kiPosition) :
Controller(period, maxSpeed, maxAcceleration)
{
    this->positionPid = new Pid(period, kpPosition, kiPosition, 0.0, maxPositionCommand);
    this->speedPid    = new Pid(period, kpSpeed, kiSpeed, 0.0, maxSpeedCommand);

    this->DefineSetPoint(0.0);
}

PositionController::~PositionController(void)
{
    delete this->positionPid;
    delete this->speedPid;
}

float PositionController::Run(float speedMeasure, float positionMeasure)
{
    // The position setpoint is the global controller setpoint
    float setPointPosition = this->GetSetPoint();
    // Get the last speed setpoint to limit the acceleration
    float lastSetPointSpeed = this->speedPid->GetSetPoint();
    // the position PID output is the setpoint of speed PID
    float setPointSpeed = this->maxSpeed * this->positionPid->Run(setPointPosition, positionMeasure);

    // We limit the speed setpoint and its derivate (acceleration)
    setPointSpeed = Maths::Limit(setPointSpeed, this->maxSpeed);
    setPointSpeed = Maths::LimitVariation(setPointSpeed, lastSetPointSpeed, this->period,
                                          this->maxAcceleration);

    // the mixer command is the output of speed PID
    return this->speedPid->Run(setPointSpeed, speedMeasure);
}