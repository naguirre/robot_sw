#ifndef __PID_H__
#define __PID_H__

#include <maths/maths.h>
#include <types.h>

class Pid
{

private:

    struct{
        float period;      //PID period 
       	float kp;
        float ki;
        struct{
            float num[2];   // Settings of discrete controller numerator (a=num[i] <=> a*z^i)
            float den[2];   // Settings of discrete controller denominator (a=num[i] <=> a*z^i)
        }discrete;
        float maxCommand;   // to limit and normalize the command
    }settings;

    struct{
        float setPoint;     // setpoint
        float measure;      // output measure
    }inputs;

    struct{
        float error;
        float lastError;    // for recurrency equation
        float command;
        float lastCommand;  // for recurrency equation
    }state;

    float output;

    void ComputeDiscrete();

    void ComputeCommand(void);

public :

    Pid(float period, float kp, float ki, float setPoint, float maxCommand);

    float Run(float setPoint, float measure);

    void  SetKp(float kp);

    void  SetKi(float ki);

    float GetKp();

    float GetKi();

    float GetSetPoint();
};


#endif /* __PID_H__ */
