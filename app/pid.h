#ifndef __PID_H__
#define __PID_H__

#include <maths/maths.h>
#include <types.h>

class Pid
{

private:

    struct{
        float period;      //period du correcteur
       	float kp;
        float ki;
        struct{
            float num[2];	//Parametres du num du correcteur (a=num[i] <=> a*z^i)
            float den[2];
        }discrete;
        float maxCommand;  //pour limitation et normalisation
    }settings;

    struct{
        float setPoint;    //consigne
        float measure;      //mesure de la sortie
    }inputs;

    struct{
        float error;
        float lastError;     //pour equations de récurrence
        float command;
        float lastCommand;   //pour equation de récurrence
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
