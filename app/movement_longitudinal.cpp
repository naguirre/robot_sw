#include <app/movement_longitudinal.h>
#include <stdio.h>

LongitudinalMovement::LongitudinalMovement(SpeedController *longitudinalSpeedController, PositionController *longitudinalPositionController,
                                           SpeedController *angularSpeedController, PositionController *angularPositionController) : 
    StateMachine(ST_MOVEMENT_STOP, ST_MOVEMENT_STOP, ST_MOVEMENT_ONGOING + 1),
    Movement(longitudinalSpeedController, longitudinalPositionController, angularSpeedController, angularPositionController)
{
}

bool LongitudinalMovement::CheckBlocked()
{
    if (Maths::InsideBoundary(this->longitudinalSpeed, LONGITUDINAL_MOVEMENT_BLOCKING_THRESHOLD))
    {
        this->blockingCount++;

        if (this->blockingCount > LONGITUDINAL_MOVEMENT_BLOCKING_NUM)
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

bool LongitudinalMovement::CheckAlmostDone()
{
    float setPoint = this->currentLongitudinalController->GetSetPoint();

    return Maths::InsideBoundary(this->longitudinalPosition - setPoint, LONGITUDINAL_MOVEMENT_ALMOST_DONE_THRESHOLD);
}

bool LongitudinalMovement::CheckDone()
{
    return Maths::InsideBoundary(this->longitudinalSpeed, LONGITUDINAL_MOVEMENT_DONE_THRESHOLD);
}

/******** State machine ***********/

void LongitudinalMovement::StateOnGoing()
{
    //On signale le changement d'etat
    if (FirstTime())
    {
        fprintf(stderr, "Longitudinal | On Going\n");
        this->blockingCount = 0;
    }

    if (CheckBlocked())
    {
        ChangeState(ST_MOVEMENT_BLOCKED, 0, LONGITUDINAL_MOVEMENT_STATE_MACHINE_TIMEOUT);
    }

    if (CheckAlmostDone())
    {
        ChangeState(ST_MOVEMENT_ALMOST_DONE, 0, LONGITUDINAL_MOVEMENT_STATE_MACHINE_TIMEOUT);
    }
}

void LongitudinalMovement::StateAlmostDone(void)
{
    if (FirstTime())
    {
        fprintf(stderr, "Longitudinal | Almost done\n");
    }

    if (CheckDone())
    {
        ChangeState(ST_MOVEMENT_DONE, 0, LONGITUDINAL_MOVEMENT_STATE_MACHINE_TIMEOUT);
    }
}

void LongitudinalMovement::StateDone(void)
{
    if (FirstTime())
    {
        fprintf(stderr, "Longitudinal| Done\n");
    }
}

void LongitudinalMovement::StateBlocked(void)
{
    if (FirstTime())
    {
        fprintf(stderr, "Longitudinal | Blocked\n");
        Stop();
    }
}

void LongitudinalMovement::StateStop(void)
{
    if (FirstTime())
    {
        fprintf(stderr, "Longitudinal | Stop\n");
    }
}

void LongitudinalMovement::Translate(float distance)
{
    this->currentLongitudinalController = this->longitudinalPositionController;
    this->currentAngularController = this->angularPositionController;

    this->currentLongitudinalController->DefineSetPoint(this->longitudinalPosition + distance);
    this->currentAngularController->DefineSetPoint(this->angularPosition);

    this->ChangeState(ST_MOVEMENT_ONGOING, 0, LONGITUDINAL_MOVEMENT_STATE_MACHINE_TIMEOUT);
}

void LongitudinalMovement::InfiniteTranslation(float speed)
{
    this->currentLongitudinalController = this->longitudinalSpeedController;
    this->currentAngularController = this->angularPositionController;

    this->currentLongitudinalController->DefineSetPoint(speed);
    this->currentAngularController->DefineSetPoint(this->angularPosition);

    this->ChangeState(ST_MOVEMENT_ONGOING, 0, LONGITUDINAL_MOVEMENT_STATE_MACHINE_TIMEOUT);
}

void LongitudinalMovement::Stop(void)
{
    this->currentLongitudinalController = this->longitudinalSpeedController;
    this->currentAngularController = this->angularPositionController;

    this->currentLongitudinalController->DefineSetPoint(this->longitudinalPosition);
    this->currentAngularController->DefineSetPoint(this->angularPosition);

    this->ChangeState(ST_MOVEMENT_STOP, 0, 0);
}

Movement::State LongitudinalMovement::GetState(void)
{
    return (Movement::State)(this->GetCurrentState());
}

void LongitudinalMovement::Run(void)
{
    Engine();
}
