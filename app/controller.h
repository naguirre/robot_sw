#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include <types.h>

class Controller
{

public:

    Controller(float period, float maxSpeed, float maxAcceleration);

    ~Controller(void);

    void DefineSetPoint(float value);

    void  SetMaxSpeed(float maxSpeed);

    float GetSetPoint();

    virtual float Run(float speedMeasure, float positionMeasure) = 0;

protected:

    float setPoint;
    
    float period;
    float maxSpeed;
    float maxAcceleration;
};

#endif /* __CONTROLLER_H__ */
