#include <app/movement_longitudinal.h>
#include <logs/logs.h>

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

    if (this->currentLongitudinalController == this->longitudinalPositionController)
    {
        return Maths::InsideBoundary(this->longitudinalPosition - setPoint, LONGITUDINAL_MOVEMENT_ALMOST_DONE_THRESHOLD);
    }
    else
    {
        // A speed controlled movement (ie infinite) is never almost done
        return false;
    }
}

bool LongitudinalMovement::CheckDone()
{
    float setPoint = this->currentLongitudinalController->GetSetPoint();

    if (this->currentLongitudinalController == this->longitudinalPositionController)
    {
        return (Maths::InsideBoundary(this->longitudinalSpeed, LONGITUDINAL_MOVEMENT_DONE_SPEED_THRESHOLD) &&
                Maths::InsideBoundary(this->longitudinalPosition - setPoint, LONGITUDINAL_MOVEMENT_DONE_POSITION_THRESHOLD));
    }
    else
    {
        /* A speed controlled movement (ie infinite) is never done */
        return false;
    }
}

/******** State machine ***********/

void LongitudinalMovement::StateOnGoing()
{
    if (FirstTime())
    {
        INF("Longitudinal | On Going");
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
        INF("Longitudinal | Almost done");
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
        INF("Longitudinal| Done");
    }
}

void LongitudinalMovement::StateBlocked(void)
{
    if (FirstTime())
    {
        INF("Longitudinal | Blocked");
        Stop();
    }
}

void LongitudinalMovement::StateStop(void)
{
    if (FirstTime())
    {
        INF("Longitudinal | Stop");
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
