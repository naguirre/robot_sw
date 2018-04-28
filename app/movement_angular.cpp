#include <app/movement_angular.h>
#include <logs/logs.h>

AngularMovement::AngularMovement(SpeedController *longitudinalSpeedController, PositionController *longitudinalPositionController,
                                 SpeedController *angularSpeedController, PositionController *angularPositionController) :
    Movement(longitudinalSpeedController, longitudinalPositionController, angularSpeedController, angularPositionController)
{
}

void AngularMovement::ComputeInitialRotation(void)
{
    return;
}

void AngularMovement::UpdateSetPoints(void)
{
    return;
}

bool AngularMovement::CheckRotationDone(void)
{
    return false;
}

bool AngularMovement::CheckRotationBlocked(void)
{
    return false;
}

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

    if (this->currentAngularController == this->angularPositionController)
    {
        return Maths::InsideBoundary(this->angularPosition - setPoint, ANGULAR_MOVEMENT_ALMOST_DONE_THRESHOLD);
    }
    else
    {
        return false;
    }
}

bool AngularMovement::CheckDone()
{
    float setPoint = this->currentAngularController->GetSetPoint();

    if (this->currentAngularController == this->angularPositionController)
    {
        return (Maths::InsideBoundary(this->angularSpeed, ANGULAR_MOVEMENT_DONE_SPEED_THRESHOLD) &&
                Maths::InsideBoundary(this->angularPosition - setPoint, ANGULAR_MOVEMENT_DONE_POSITION_THRESHOLD));
    }
    else
    {
        return false;
    }
}

void AngularMovement::Rotate(float angle)
{
    this->currentLongitudinalController = this->longitudinalPositionController;
    this->currentAngularController = this->angularPositionController;

    this->currentLongitudinalController->SetMaxSpeed(LONGITUDINAL_MOVEMENT_SPEED_MAX);
    this->currentLongitudinalController->DefineSetPoint(this->longitudinalPosition);
    this->currentAngularController->SetMaxSpeed(ANGULAR_MOVEMENT_SPEED_MAX);
    this->currentAngularController->DefineSetPoint(this->angularPosition + angle);

    ChangeState(ST_MOVEMENT_ONGOING, 0, MOVEMENT_STATE_MACHINE_TIMEOUT);
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

void AngularMovement::InfiniteRotation(float speed)
{
    this->currentLongitudinalController = this->longitudinalPositionController;
    this->currentAngularController = this->angularSpeedController;

    this->currentLongitudinalController->SetMaxSpeed(LONGITUDINAL_MOVEMENT_SPEED_MAX);
    this->currentLongitudinalController->DefineSetPoint(this->longitudinalPosition);
    this->currentAngularController->DefineSetPoint(speed);

    ChangeState(ST_MOVEMENT_ONGOING, 0, MOVEMENT_STATE_MACHINE_TIMEOUT);
}
