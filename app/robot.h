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
#include <app/movement_complex.h>

#include <simu/SimuMotor.h>

class Robot
{

private:

    Odometry             * odometry;
    Mixer                * mixer;
    PositionController   * longitudinalPositionController;
    PositionController   * angularPositionController;
    SpeedController      * longitudinalSpeedController;
    SpeedController      * angularSpeedController;
    Movement             * currentMovement;
    AngularMovement      * angularMovement;
    LongitudinalMovement * longitudinalMovement;
    GoToMovement         * goToMovement;
    SplittedGoToMovement * splittedGoToMovement;
    GoThroughMovement    * goThroughMovement;
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

    void PointTowards(float x, float y);

    void GoTo(float x, float y, float speed);

    void SplittedGoTo(float x, float y, float speed);

    void GoThrough(float x, float y, float speed);

    void Stop(void);

    bool CheckMovementOngoing();

    bool CheckMovementAlmostDone();

    bool CheckMovementDone();

    bool CheckMovementBlocked();
};

#endif  /* __ROBOT_H__ */
