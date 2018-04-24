#ifndef __SIMU_MOTOR_H__
#define __SIMU_MOTOR_H__

#include <types.h>

class SimuMotor
{

private:

    struct
    {
        float timeConstant;
        float staticGain;
        float period;
        float voltage;
    } settings;

    struct
    {
        float command;
        float speed;
        float lastSpeed;
        float position;
        bool  block;
        int   blockCount;
    } state;

public:

    SimuMotor(float periodStep);

    void SetCommand(float duty);

    float GetAngle(void);

    float GetSpeed();

    void Run(void);
};



#endif