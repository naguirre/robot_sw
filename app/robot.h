#ifndef __ROBOT_H__
#define __ROBOT_H__

#include <types.h>

#include <app/movement.h>
#include <app/controller.h>
#include <app/controller_speed.h>
#include <app/controller_position.h>
#include <app/odometry.h>
#include <app/mixer.h>
#include <app/movement_longitudinal.h>
#include <app/movement_angular.h>

#include <simu/SimuMotor.h>

class Robot
{

private:

    Odometry             * odometry;
    Mixer                * mixer;
    Movement             * movement;
    PositionController   * longitudinalPositionController;
    PositionController   * angularPositionController;
    SpeedController      * longitudinalSpeedController;
    SpeedController      * angularSpeedController;
    Controller           * longitudinalController;
    Controller           * angularController;
    Movement             * currentMovement;
    AngularMovement      * angularMovement;
    LongitudinalMovement * longitudinalMovement;
    SimuMotor            * simuMotors[2];

public:

    typedef enum{
        MOTOR_RIGHT,
        MOTOR_LEFT,
        MOTOR_MAX,
    }Motor;

    Robot(float period, bool simu);
    
    void Run(void);

    void Translate(float distance);

    void Rotate(float angle);

    void Stop(void);

    bool CheckMovementOngoing();

    bool CheckMovementDone();

    bool CheckMovementBlocked();
};

#endif  /* __ROBOT_H__ */
