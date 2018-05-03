#include <app/movement_longitudinal.h>
#include <logs/logs.h>

LongitudinalMovement::LongitudinalMovement(SpeedController *longitudinalSpeedController, PositionController *longitudinalPositionController,
                                           SpeedController *angularSpeedController, PositionController *angularPositionController) : 
    Movement(longitudinalSpeedController, longitudinalPositionController, angularSpeedController, angularPositionController)
{
}

void LongitudinalMovement::ComputeInitialRotation(void)
{
    return;
}

void LongitudinalMovement::UpdateSetPoints(void)
{
    return;
}

bool LongitudinalMovement::CheckRotationDone(void)
{
    return false;
}

bool LongitudinalMovement::CheckRotationBlocked(void)
{
    return false;
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

void LongitudinalMovement::Translate(float distance)
{
    this->currentLongitudinalController = this->longitudinalPositionController;
    this->currentAngularController = this->angularPositionController;

    this->currentLongitudinalController->SetMaxSpeed(LONGITUDINAL_MOVEMENT_SPEED_MAX);
    this->currentLongitudinalController->DefineSetPoint(this->longitudinalPosition + distance);
    this->currentAngularController->SetMaxSpeed(ANGULAR_MOVEMENT_SPEED_MAX);
    this->currentAngularController->DefineSetPoint(this->angularPosition);

    this->ChangeState(ST_MOVEMENT_ONGOING, 0, MOVEMENT_STATE_MACHINE_TIMEOUT);
}

void LongitudinalMovement::InfiniteTranslation(float speed)
{
    this->currentLongitudinalController = this->longitudinalSpeedController;
    this->currentAngularController = this->angularPositionController;

    this->currentLongitudinalController->DefineSetPoint(speed);
    this->currentAngularController->SetMaxSpeed(ANGULAR_MOVEMENT_SPEED_MAX);
    this->currentAngularController->DefineSetPoint(this->angularPosition);

    this->ChangeState(ST_MOVEMENT_ONGOING, 0, MOVEMENT_STATE_MACHINE_TIMEOUT);
}

