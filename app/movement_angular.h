#ifndef __ANGULAR_MOVEMENT_H__
#define __ANGULAR_MOVEMENT_H__

/*************************************/
/* INCLUDES                          */
/*************************************/

#include <types.h>
#include <math.h>

#include <maths/maths.h>

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
#define ANGULAR_MOVEMENT_DONE_SPEED_THRESHOLD       0.08    // 1°/s
#define ANGULAR_MOVEMENT_BLOCKING_THRESHOLD         0.005
#define ANGULAR_MOVEMENT_BLOCKING_NUM               10

class AngularMovement : public Movement
{

private:

    void ComputeInitialRotation(void);
    void UpdateSetPoints(void);
    bool CheckRotationDone(void);
    bool CheckRotationBlocked(void);
    bool CheckBlocked();
    bool CheckAlmostDone();
    bool CheckDone();

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

    void InfiniteRotation(float speed);
};

#endif /* __ANGULAR_MOVEMENT_H__ */
