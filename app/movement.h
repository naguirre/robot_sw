#ifndef __MOVEMENT_H__
#define __MOVEMENT_H__

#include <state_machine/StateMachine.h>
#include <app/controller.h>
#include <app/controller_position.h>
#include <app/controller_speed.h>

// the generic movement state machine class
class Movement
{

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
    virtual Movement::State GetState() = 0;

    virtual void Run() = 0;

    void Update(float longitudinalPosition, float longitudinalSpeed, float angularPosition, float angularSpeed,
                float x, float y, float heading);

    float GetAngularCommand();

    float GetLongitudinalCommand();

};

#endif /* __MOVEMENT_H__ */