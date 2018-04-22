#ifndef __CONTROLLER_SPEED_H__
#define __CONTROLLER_SPEED_H__

#include <types.h>
#include <app/controller.h>
#include <app/pid.h>

class SpeedController : public Controller
{

private:

    Pid * speedPid;

public:
    
    SpeedController(float period, float maxSpeed, float maxAcceleration,
                    float maxSpeedCommand, float kpSpeed, float kiSpeed);

    ~SpeedController();

    virtual float Run(float speedMeasure, float positionMeasure);
};

#endif /* __CONTROLLER_SPEED_H__ */
