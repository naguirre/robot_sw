#ifndef __MIXER_H__
#define __MIXER_H__

/*************************************/
/* DEFINES                           */
/*************************************/

#include <types.h>
#include <maths/maths.h>
#include <hal/h_bridge.h>

/*************************************/
/* FONCTIONS                         */
/*************************************/

class Mixer
{

private:

    HBridge * hBridges[2];

    void ApplyRightCommand(float command);

    void ApplyLeftCommand(float command);

public:

    Mixer();

    ~Mixer();

    void ApplyRightLeftCommand(float right, float left);

    void ApplyPolarCommand(float longitudinal, float angular);

};

#endif /* __MIXER_H__ */
