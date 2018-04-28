#ifndef __MOVEMENT_H__
#define __MOVEMENT_H__

#include <state_machine/StateMachine.h>
#include <app/controller.h>
#include <app/controller_position.h>
#include <app/controller_speed.h>

#define MOVEMENT_STATE_MACHINE_TIMEOUT     5000    //5 secondes

#define ANGULAR_MOVEMENT_SPEED_MAX         10.0     // 1 t/s
#define LONGITUDINAL_MOVEMENT_SPEED_MAX    1.2     // m/s

// the generic movement state machine class
class Movement : public StateMachine
{

private:

    void StateOnGoing();
    void StateAlmostDone(void);
    void StateDone(void);
    void StateBlocked(void);
    void StateStop(void);
    void StateInitialRotation(void);

    BEGIN_STATE_MAP
    STATE_MAP_ENTRY(Movement::StateStop)
    STATE_MAP_ENTRY(Movement::StateBlocked)
    STATE_MAP_ENTRY(Movement::StateDone)
    STATE_MAP_ENTRY(Movement::StateAlmostDone)
    STATE_MAP_ENTRY(Movement::StateOnGoing)
    STATE_MAP_ENTRY(Movement::StateInitialRotation)
    END_STATE_MAP

protected:

    int   blockingCount;
    float angularCommand;
    float longitudinalCommand;
    float longitudinalPosition;
    float longitudinalSpeed;
    float angularPosition;
    float angularSpeed;
    float x;
    float y;
    float heading;

    SpeedController    * longitudinalSpeedController;
    PositionController * longitudinalPositionController;
    SpeedController    * angularSpeedController;
    PositionController * angularPositionController;
    Controller         * currentLongitudinalController;
    Controller         * currentAngularController;

    virtual void ComputeInitialRotation(void) = 0;

    virtual void UpdateSetPoints(void) = 0;

    virtual bool CheckRotationDone(void) = 0;

    virtual bool CheckRotationBlocked(void) = 0;

    virtual bool CheckBlocked() = 0;

    virtual bool CheckAlmostDone() = 0;
    
    virtual bool CheckDone() = 0;

public:

    typedef enum {
        ST_MOVEMENT_STOP = 0,
        ST_MOVEMENT_BLOCKED,
        ST_MOVEMENT_DONE,
        ST_MOVEMENT_ALMOST_DONE,
        ST_MOVEMENT_ONGOING,
        ST_MOVEMENT_INITIAL_ROTATION,
        ST_MOVEMENT_MAX
    }State;

    Movement(SpeedController *longitudinalSpeedController, PositionController *longitudinalPositionController,
             SpeedController *angularSpeedController, PositionController *angularPositionController);

    // external events taken by this state machine
    Movement::State GetState();

    void Run();

    void Stop();

    void Update(float longitudinalPosition, float longitudinalSpeed, float angularPosition, float angularSpeed,
                float x, float y, float heading);

    float GetAngularCommand();

    float GetLongitudinalCommand();

};

#endif /* __MOVEMENT_H__ */