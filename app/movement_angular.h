#ifndef __ANGULAR_MOVEMENT_H__
#define __ANGULAR_MOVEMENT_H__

/*************************************/
/* INCLUDES                          */
/*************************************/

#include <types.h>
#include <math.h>

#include <maths/maths.h>
#include <state_machine/StateMachine.h>

#include <app/movement.h>
#include <app/controller.h>
#include <app/controller_position.h>
#include <app/controller_speed.h>
#include <app/odometry.h>
#include <app/mixer.h>

/*************************************/
/* DEFINES                           */
/*************************************/

#define ANGULAR_MOVEMENT_ALMOST_DONE_THRESHOLD      0.175   // 10°
#define ANGULAR_MOVEMENT_DONE_POSITION_THRESHOLD    0.0175  // 1°
#define ANGULAR_MOVEMENT_DONE_SPEED_THRESHOLD       0.04    // 0.5°/s
#define ANGULAR_MOVEMENT_BLOCKING_THRESHOLD         0.005
#define ANGULAR_MOVEMENT_BLOCKING_NUM               10

#define ANGULAR_MOVEMENT_STATE_MACHINE_TIMEOUT  5000    // 5 seconds

class AngularMovement : public StateMachine, public Movement
{

private:

    bool CheckBlocked();
    bool CheckAlmostDone();
    bool CheckDone();

    void StateOnGoing();
    void StateAlmostDone(void);
    void StateDone(void);
    void StateBlocked(void);
    void StateStop(void);

    BEGIN_STATE_MAP
        STATE_MAP_ENTRY(AngularMovement::StateStop)
        STATE_MAP_ENTRY(AngularMovement::StateBlocked)
        STATE_MAP_ENTRY(AngularMovement::StateDone)
        STATE_MAP_ENTRY(AngularMovement::StateAlmostDone)
        STATE_MAP_ENTRY(AngularMovement::StateOnGoing)
    END_STATE_MAP

public:

    typedef enum
    {
        CLOCKWISE,
        COUNTERCLOCKWISE,
    }Direction;

    AngularMovement(SpeedController *longitudinalSpeedController, PositionController *longitudinalPositionController,
                    SpeedController *angularSpeedController, PositionController *angularPositionController);

    void Rotate(float angle);

    void PointTowards(float x, float y);

    void PointTowardsBack(float x, float y);

    void TangentCircle(float x, float y, AngularMovement::Direction direction);

    void InfiniteRotation(float speed);

    void Stop(void);

    virtual Movement::State GetState(void);

    virtual void Run(void);
};

#endif /* __ANGULAR_MOVEMENT_H__ */
