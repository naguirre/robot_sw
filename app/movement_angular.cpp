#include <app/movement_angular.h>
#include <stdio.h>


AngularMovement::AngularMovement(SpeedController *longitudinalSpeedController, PositionController *longitudinalPositionController,
                                 SpeedController *angularSpeedController, PositionController *angularPositionController) :
    StateMachine(ST_MOVEMENT_STOP, ST_MOVEMENT_STOP, ST_MOVEMENT_ONGOING + 1), 
    Movement(longitudinalSpeedController, longitudinalPositionController, angularSpeedController, angularPositionController)
{
}

/******** CONDITIONS DE TRANSITIONS ***********/

bool AngularMovement::CheckBlocked()
{
    if(Maths::InsideBoundary(this->angularSpeed, ANGULAR_MOVEMENT_BLOCKING_THRESHOLD))
    {
        this->blockingCount++;

        if (this->blockingCount > ANGULAR_MOVEMENT_BLOCKING_NUM)
        {
            return true;
        }
    }
    else if (this->blockingCount > 0)
    {
        this->blockingCount--;
    }

    return false;
}

bool AngularMovement::CheckAlmostDone()
{
    float setPoint = this->currentAngularController->GetSetPoint();

    return Maths::InsideBoundary(this->angularPosition - setPoint, ANGULAR_MOVEMENT_ALMOST_DONE_THRESHOLD);
}

bool AngularMovement::CheckDone()
{
    return Maths::InsideBoundary(this->angularSpeed, ANGULAR_MOVEMENT_DONE_THRESHOLD);
}

/******** State machine ***********/


void AngularMovement::StateOnGoing()
{
    //On signale le changement d'etat
    if (FirstTime())
    {
        fprintf(stderr, "Angular | On Going\n");
        this->blockingCount = 0;
    }

    if(CheckBlocked())
    {
        ChangeState(ST_MOVEMENT_BLOCKED, 0, ANGULAR_MOVEMENT_STATE_MACHINE_TIMEOUT);
    }
    
    if (CheckAlmostDone())
    {
        ChangeState(ST_MOVEMENT_ALMOST_DONE, 0, ANGULAR_MOVEMENT_STATE_MACHINE_TIMEOUT);
    }
}

void AngularMovement::StateAlmostDone(void)
{
    if (FirstTime())
    {
        fprintf(stderr, "Angular | Almost done\n");
    }
    
    if (CheckDone())
    {
        ChangeState(ST_MOVEMENT_DONE, 0, ANGULAR_MOVEMENT_STATE_MACHINE_TIMEOUT);
    }
}

void AngularMovement::StateDone(void)
{
    if (FirstTime())
    {
        fprintf(stderr, "Angular| Done\n");
    }
}

void AngularMovement::StateBlocked(void)
{
    if (FirstTime())
    {
        Stop();
        fprintf(stderr, "Angular | Blocked\n");
    }
}

void AngularMovement::StateStop(void)
{
    if (FirstTime())
    {
        fprintf(stderr, "Angular | Stop\n");
    }
}

void AngularMovement::Rotate(float angle)
{
    this->currentLongitudinalController = this->longitudinalPositionController;
    this->currentAngularController = this->angularPositionController;

    this->currentLongitudinalController->DefineSetPoint(this->longitudinalPosition);
    this->currentAngularController->DefineSetPoint(this->angularPosition + angle);

    this->ChangeState(ST_MOVEMENT_ONGOING, 0, ANGULAR_MOVEMENT_STATE_MACHINE_TIMEOUT);
}

void AngularMovement::PointTowards(float x, float y)
{
    float angle = Maths::DeltaHeading(this->heading, this->x, this->y, x, y);

    Rotate(angle);
}

void AngularMovement::PointTowardsBack(float x, float y)
{
    float angle = Maths::DeltaHeading(this->heading, this->x, this->y, x, y);

    Rotate(angle);
}

void AngularMovement::TangentCircle(float x, float y, AngularMovement::Direction direction)
{
    float angle;

    if (direction == AngularMovement::COUNTERCLOCKWISE)
    {
        angle = Maths::mod2pi(Maths::DeltaHeading(this->heading, this->x, this->y, x, y) - M_PI_2);
    }
    else
    {
        angle = Maths::mod2pi(Maths::DeltaHeading(this->heading, this->x, this->y, x, y) + M_PI_2);
    }

    Rotate(angle);
}

void AngularMovement::InfiniteRotation(float speed)
{
    this->currentLongitudinalController = this->longitudinalPositionController;
    this->currentAngularController = this->angularSpeedController;

    this->currentLongitudinalController->DefineSetPoint(this->longitudinalPosition);
    this->currentAngularController->DefineSetPoint(speed);

    this->ChangeState(ST_MOVEMENT_ONGOING, 0, ANGULAR_MOVEMENT_STATE_MACHINE_TIMEOUT);
}

void AngularMovement::Stop(void)
{
    this->currentLongitudinalController = this->longitudinalPositionController;
    this->currentAngularController = this->angularPositionController;

    this->currentLongitudinalController->DefineSetPoint(this->longitudinalPosition);
    this->currentAngularController->DefineSetPoint(this->angularPosition);

    this->ChangeState(ST_MOVEMENT_STOP, 0, 0);
}

Movement::State AngularMovement::GetState(void)
{
    return (Movement::State)(this->GetCurrentState());
}

void AngularMovement::Run(void)
{
    Engine();
}