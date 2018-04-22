#ifndef __CONTROLLER_POSITION_H__
#define __CONTROLLER_POSITION_H__

#include <types.h>
#include <app/controller.h>
#include <app/pid.h>

class PositionController : public Controller
{

private:

    Pid * speedPid;
    Pid * positionPid;

public:

    PositionController(float period, float maxSpeed, float maxAcceleration,
                       float maxSpeedCommand, float kpSpeed, float kiSpeed,
                       float maxPositionCommand, float kpPosition, float kiPosition);

    ~PositionController();

    virtual float Run(float speedMeasure, float positionMeasure);
};

#endif /* __CONTROLLER_POSITION_H__ */
