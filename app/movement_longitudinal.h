#ifndef __MOVEMENT_LONGITUNDINAL_H__
#define __MOVEMENT_LONGITUNDINAL_H__

/*************************************/
/* INCLUDES                          */
/*************************************/

#include <types.h>

#include <maths/maths.h>
#include <app/controller.h>
#include <app/controller_position.h>
#include <app/controller_speed.h>
#include <app/movement.h>
#include <app/odometry.h>
#include <app/mixer.h>

#define LONGITUDINAL_MOVEMENT_ALMOST_DONE_THRESHOLD     0.05   // 5cm
#define LONGITUDINAL_MOVEMENT_DONE_SPEED_THRESHOLD      0.05   // 5cm/s
#define LONGITUDINAL_MOVEMENT_DONE_POSITION_THRESHOLD   0.01   // 1cm/s
#define LONGITUDINAL_MOVEMENT_BLOCKING_THRESHOLD        0.005  // 5mm/s
#define LONGITUDINAL_MOVEMENT_BLOCKING_NUM              10

class LongitudinalMovement : public Movement
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
        BACKWARD,
        FORWARD,
    }Direction;

    LongitudinalMovement(SpeedController *longitudinalSpeedController, PositionController *longitudinalPositionController,
                         SpeedController *angularSpeedController, PositionController *angularPositionController);

    void Translate(float distance);

    void InfiniteTranslation(float speed);

};

#endif /* __MOVEMENT_LONGITUNDINAL_H__ */
