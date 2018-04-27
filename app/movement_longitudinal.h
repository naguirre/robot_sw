#ifndef __MOVEMENT_LONGITUNDINAL_H__
#define __MOVEMENT_LONGITUNDINAL_H__

/*************************************/
/* INCLUDES                          */
/*************************************/

#include <types.h>

#include <maths/maths.h>
#include <state_machine/StateMachine.h>
#include <app/controller.h>
#include <app/controller_position.h>
#include <app/controller_speed.h>
#include <app/movement.h>
#include <app/odometry.h>
#include <app/mixer.h>


#define LONGITUDINAL_MOVEMENT_ALMOST_DONE_THRESHOLD     0.05   // 5cm
#define LONGITUDINAL_MOVEMENT_DONE_SPEED_THRESHOLD      0.01   // 1cm/s
#define LONGITUDINAL_MOVEMENT_DONE_POSITION_THRESHOLD   0.01   // 1cm/s
#define LONGITUDINAL_MOVEMENT_BLOCKING_THRESHOLD        0.005  // 5mm/s
#define LONGITUDINAL_MOVEMENT_BLOCKING_NUM              10

#define LONGITUDINAL_MOVEMENT_STATE_MACHINE_TIMEOUT     5000    //5 secondes

class LongitudinalMovement : public StateMachine, public Movement
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
    STATE_MAP_ENTRY(LongitudinalMovement::StateStop)
    STATE_MAP_ENTRY(LongitudinalMovement::StateBlocked)
    STATE_MAP_ENTRY(LongitudinalMovement::StateDone)
    STATE_MAP_ENTRY(LongitudinalMovement::StateAlmostDone)
    STATE_MAP_ENTRY(LongitudinalMovement::StateOnGoing)
    END_STATE_MAP

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

    void Stop(void);

    Movement::State GetState(void);

    void Run(void);
};

#endif /* __MOVEMENT_LONGITUNDINAL_H__ */
