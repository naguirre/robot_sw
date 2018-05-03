#ifndef __COMPLEX_MOVEMENT_H__
#define __COMPLEX_MOVEMENT_H__

#include <types.h>
#include <math.h>

#include <maths/maths.h>
#include <state_machine/StateMachine.h>

#include <app/movement.h>
#include <app/movement_longitudinal.h>
#include <app/movement_angular.h>
#include <app/controller.h>
#include <app/controller_position.h>
#include <app/controller_speed.h>
#include <app/odometry.h>
#include <app/mixer.h>

#define COMPLEX_MOVEMENT_STATE_MACHINE_TIMEOUT 5000

class ComplexMovement : public Movement
{

protected:

    ComplexMovement(SpeedController * longitudinalSpeedController, PositionController * longitudinalPositionController,
                    SpeedController * angularSpeedController, PositionController * angularPositionController);


    bool TargetPointAhead(float x, float y, float speed);

    bool CheckRotationDone(void);

    bool CheckRotationBlocked(void);

    bool CheckBlocked();

    bool CheckDone();

};

class GoToMovement : public ComplexMovement
{

private:

    float targetX;
    float targetY;
    float targetSpeed;

    void ComputeInitialRotation(void);

    void UpdateSetPoints(void);

    bool CheckAlmostDone(void);

public:

    GoToMovement(SpeedController * longitudinalSpeedController, PositionController * longitudinalPositionController,
                 SpeedController * angularSpeedController, PositionController * angularPositionController);

    void GoTo(float x, float y, float speed);

};

class SplittedGoToMovement : public ComplexMovement
{

private:

    float targetX;
    float targetY;
    float targetSpeed;

    void ComputeInitialRotation(void);

    void UpdateSetPoints(void);

    bool CheckAlmostDone(void);

public:

    SplittedGoToMovement(SpeedController * longitudinalSpeedController, PositionController * longitudinalPositionController,
                         SpeedController * angularSpeedController, PositionController * angularPositionController);

    void GoTo(float x, float y, float speed);

};

class GoThroughMovement : public ComplexMovement
{

private:

    float targetX;
    float targetY;
    float targetSpeed;

    void ComputeInitialRotation(void);

    void UpdateSetPoints(void);

    bool CheckAlmostDone(void);

public:

    GoThroughMovement(SpeedController * longitudinalSpeedController, PositionController * longitudinalPositionController,
                      SpeedController * angularSpeedController, PositionController * angularPositionController);

    void GoThrough(float x, float y, float speed);

};

class RotationAroundPointMovement : public ComplexMovement
{

private:

    float centerX;
    float centerY;
    AngularMovement::Direction direction;
    float speed;
    float radius;

    void ComputeInitialRotation(void);

    void UpdateSetPoints(void);

    bool CheckAlmostDone(void);

public:

    RotationAroundPointMovement(SpeedController * longitudinalSpeedController, PositionController * longitudinalPositionController,
                                SpeedController * angularSpeedController, PositionController * angularPositionController);

    void RotateAround(float x, float y, AngularMovement::Direction direction, float speed);

};

#endif /* __COMPLEX_MOVEMENT_H__ */