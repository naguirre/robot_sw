#ifndef __MIXER_H__
#define __MIXER_H__

/*************************************/
/* DEFINES                           */
/*************************************/

#include <types.h>
#include <maths/maths.h>
#include <hal/h_bridge.h>

#include <simu/SimuMotor.h>

/*************************************/
/* FONCTIONS                         */
/*************************************/

class Mixer
{

private:

    HBridge   * hBridges[2];
    SimuMotor * simuMotors[2];

    void ApplyRightCommand(float command);

    void ApplyLeftCommand(float command);

public:

    Mixer();

    Mixer(SimuMotor * simuMotorRight, SimuMotor * simuMotorLeft);

    ~Mixer();

    void ApplyRightLeftCommand(float right, float left);

    void ApplyPolarCommand(float longitudinal, float angular);

};

#endif /* __MIXER_H__ */
