#include <app/movement_complex.h>
#include <stdio.h>
#include <math.h>
#include <logs/logs.h>

ComplexMovement::ComplexMovement(SpeedController * longitudinalSpeedController, PositionController * longitudinalPositionController,
                                 SpeedController * angularSpeedController, PositionController * angularPositionController) : 
    Movement(longitudinalSpeedController, longitudinalPositionController, angularSpeedController, angularPositionController)
{
}

bool ComplexMovement::TargetPointAhead(float x, float y, float speed)
{
    float currentX     = this->x;
    float currentY     = this->y;
    float currentAngle = this->heading;

    if (speed >= 0.0)
    {
        return (bool)(Maths::Abs(Maths::DeltaHeading(currentAngle, currentX, currentY, x, y)) < M_PI_2);
    }
    else
    {
        return !(bool)(Maths::Abs(Maths::DeltaHeading(currentAngle, currentX, currentY, x, y)) < M_PI_2);
    }
}

bool ComplexMovement::CheckRotationDone(void)
{
    float setPoint = this->angularPositionController->GetSetPoint();

    return (Maths::InsideBoundary(this->angularSpeed, ANGULAR_MOVEMENT_DONE_SPEED_THRESHOLD) &&
            Maths::InsideBoundary(this->angularPosition - setPoint, ANGULAR_MOVEMENT_DONE_POSITION_THRESHOLD));
}

bool ComplexMovement::CheckRotationBlocked(void)
{
    if (Maths::InsideBoundary(this->angularSpeed, ANGULAR_MOVEMENT_BLOCKING_THRESHOLD))
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

bool ComplexMovement::CheckBlocked(void)
{
    if (Maths::InsideBoundary(this->longitudinalSpeed, LONGITUDINAL_MOVEMENT_BLOCKING_THRESHOLD))
    {
        if (++this->blockingCount > LONGITUDINAL_MOVEMENT_BLOCKING_NUM)
        {
            return true;
        }
    }
    return false;
}

bool ComplexMovement::CheckDone()
{
    if (this->currentLongitudinalController == this->longitudinalPositionController)
    {
        return (Maths::InsideBoundary(this->angularSpeed, ANGULAR_MOVEMENT_DONE_SPEED_THRESHOLD) &&
                Maths::InsideBoundary(this->longitudinalSpeed, LONGITUDINAL_MOVEMENT_DONE_SPEED_THRESHOLD));
    }
    else
    {
        /* A speed controlled movement (ie infinite) is never done */
        return false;
    }
}

/* GoToMovement */

GoToMovement::GoToMovement(SpeedController * longitudinalSpeedController, PositionController * longitudinalPositionController,
                           SpeedController * angularSpeedController, PositionController * angularPositionController):
    ComplexMovement(longitudinalSpeedController, longitudinalPositionController, angularSpeedController, angularPositionController),
    targetX(0.0),
    targetY(0.0),
    targetSpeed(0.0)
{
}

void GoToMovement::ComputeInitialRotation(void)
{
    float angle;
    
    if (this->targetSpeed >= 0.0)
    {
        angle = Maths::DeltaHeading(this->heading, this->x, this->y, this->targetX, this->targetY);
    }
    else
    {
        angle = Maths::DeltaHeadingReverse(this->heading, this->x, this->y, this->targetX, this->targetY);
    }

    this->currentLongitudinalController = this->longitudinalPositionController;
    this->currentAngularController = this->angularPositionController;

    this->currentLongitudinalController->SetMaxSpeed(LONGITUDINAL_MOVEMENT_SPEED_MAX);
    this->currentLongitudinalController->DefineSetPoint(this->longitudinalPosition);
    this->currentAngularController->SetMaxSpeed(ANGULAR_MOVEMENT_SPEED_MAX);
    this->currentAngularController->DefineSetPoint(this->angularPosition + angle);
}

void GoToMovement::UpdateSetPoints(void)
{
    float longitudinalSetPoint;
    float angularSetPoint;

    if (this->targetSpeed >= 0.0)
    {
        angularSetPoint = this->angularPosition + Maths::DeltaHeading(this->heading, this->x, this->y, this->targetX, this->targetY);
    }
    else
    {
        angularSetPoint = this->angularPosition + Maths::DeltaHeadingReverse(this->heading, this->x, this->y, this->targetX, this->targetY);
    }

    if (Maths::Abs(Maths::DeltaHeading(this->heading, this->x, this->y, this->targetX, this->targetY)) < M_PI_2)
    {
        longitudinalSetPoint = this->longitudinalPosition + Maths::Distance(this->x, this->y, this->targetX, this->targetY);
    }
    else
    {
        longitudinalSetPoint = this->longitudinalPosition - Maths::Distance(this->x, this->y, this->targetX, this->targetY);
    }

    this->currentLongitudinalController = this->longitudinalPositionController;
    this->currentAngularController = this->angularPositionController;

    this->currentLongitudinalController->SetMaxSpeed(this->targetSpeed);
    this->currentLongitudinalController->DefineSetPoint(longitudinalSetPoint);
    this->currentAngularController->SetMaxSpeed(ANGULAR_MOVEMENT_SPEED_MAX);
    this->currentAngularController->DefineSetPoint(angularSetPoint);
}

bool GoToMovement::CheckAlmostDone(void)
{
    if (Maths::Distance(this->x, this->y, this->targetX, this->targetY) < 0.05)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void GoToMovement::GoTo(float x, float y, float speed)
{
    this->targetX     = x;
    this->targetY     = y;
    this->targetSpeed = speed;

    if (!TargetPointAhead(x, y, speed))
    {
        ChangeState(ST_MOVEMENT_INITIAL_ROTATION, 0, COMPLEX_MOVEMENT_STATE_MACHINE_TIMEOUT);
    }
    else
    {
        ChangeState(ST_MOVEMENT_ONGOING, 0, COMPLEX_MOVEMENT_STATE_MACHINE_TIMEOUT);
    }
}

/* SplittedGoToMovement */

SplittedGoToMovement::SplittedGoToMovement(SpeedController * longitudinalSpeedController, PositionController * longitudinalPositionController,
                     SpeedController * angularSpeedController, PositionController * angularPositionController):
    ComplexMovement(longitudinalSpeedController, longitudinalPositionController, angularSpeedController, angularPositionController),
    targetX(0.0),
    targetY(0.0),
    targetSpeed(0.0)
{
}

void SplittedGoToMovement::ComputeInitialRotation(void)
{
    float angle;
    
    if (this->targetSpeed >= 0.0)
    {
        angle = Maths::DeltaHeading(this->heading, this->x, this->y, this->targetX, this->targetY);
    }
    else
    {
        angle = Maths::DeltaHeadingReverse(this->heading, this->x, this->y, this->targetX, this->targetY);
    }

    this->currentLongitudinalController = this->longitudinalPositionController;
    this->currentAngularController = this->angularPositionController;

    this->currentLongitudinalController->SetMaxSpeed(LONGITUDINAL_MOVEMENT_SPEED_MAX);
    this->currentLongitudinalController->DefineSetPoint(this->longitudinalPosition);
    this->currentAngularController->SetMaxSpeed(ANGULAR_MOVEMENT_SPEED_MAX);
    this->currentAngularController->DefineSetPoint(this->angularPosition + angle);
}

void SplittedGoToMovement::UpdateSetPoints(void)
{
    float longitudinalSetPoint;
    float angularSetPoint;

    if (this->targetSpeed >= 0.0)
    {
        angularSetPoint = this->angularPosition + Maths::DeltaHeading(this->heading, this->x, this->y, this->targetX, this->targetY);
    }
    else
    {
        angularSetPoint = this->angularPosition + Maths::DeltaHeadingReverse(this->heading, this->x, this->y, this->targetX, this->targetY);
    }

    if (Maths::Abs(Maths::DeltaHeading(this->heading, this->x, this->y, this->targetX, this->targetY)) < M_PI_2)
    {
        longitudinalSetPoint = this->longitudinalPosition + Maths::Distance(this->x, this->y, this->targetX, this->targetY);
    }
    else
    {
        longitudinalSetPoint = this->longitudinalPosition - Maths::Distance(this->x, this->y, this->targetX, this->targetY);
    }

    this->currentLongitudinalController = this->longitudinalPositionController;
    this->currentAngularController = this->angularPositionController;

    this->currentLongitudinalController->SetMaxSpeed(this->targetSpeed);
    this->currentLongitudinalController->DefineSetPoint(longitudinalSetPoint);
    this->currentAngularController->SetMaxSpeed(ANGULAR_MOVEMENT_SPEED_MAX);
    this->currentAngularController->DefineSetPoint(angularSetPoint);
}

bool SplittedGoToMovement::CheckAlmostDone(void)
{
    if (Maths::Distance(this->x, this->y, this->targetX, this->targetY) < 0.05)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void SplittedGoToMovement::GoTo(float x, float y, float speed)
{
    this->targetX     = x;
    this->targetY     = y;
    this->targetSpeed = speed;

    ChangeState(ST_MOVEMENT_INITIAL_ROTATION, 0, COMPLEX_MOVEMENT_STATE_MACHINE_TIMEOUT);
}

/* GoTrough */

GoThroughMovement::GoThroughMovement(SpeedController * longitudinalSpeedController, PositionController * longitudinalPositionController,
                 SpeedController * angularSpeedController, PositionController * angularPositionController):
    ComplexMovement(longitudinalSpeedController, longitudinalPositionController, angularSpeedController, angularPositionController),
    targetX(0.0),
    targetY(0.0),
    targetSpeed(0.0)
{
}

void GoThroughMovement::ComputeInitialRotation(void)
{
    float angle;
    
    if (this->targetSpeed >= 0.0)
    {
        angle = Maths::DeltaHeading(this->heading, this->x, this->y, this->targetX, this->targetY);
    }
    else
    {
        angle = Maths::DeltaHeadingReverse(this->heading, this->x, this->y, this->targetX, this->targetY);
    }

    this->currentLongitudinalController = this->longitudinalPositionController;
    this->currentAngularController = this->angularPositionController;

    this->currentLongitudinalController->SetMaxSpeed(LONGITUDINAL_MOVEMENT_SPEED_MAX);
    this->currentLongitudinalController->DefineSetPoint(this->longitudinalPosition);
    this->currentAngularController->SetMaxSpeed(ANGULAR_MOVEMENT_SPEED_MAX);
    this->currentAngularController->DefineSetPoint(this->angularPosition + angle);
}

void GoThroughMovement::UpdateSetPoints(void)
{
    float angularSetPoint;

    if (this->targetSpeed >= 0.0)
    {
        angularSetPoint = this->angularPosition + Maths::DeltaHeading(this->heading, this->x, this->y, this->targetX, this->targetY);
    }
    else
    {
        angularSetPoint = this->angularPosition + Maths::DeltaHeadingReverse(this->heading, this->x, this->y, this->targetX, this->targetY);
    }

    this->currentLongitudinalController = this->longitudinalSpeedController;
    this->currentAngularController = this->angularPositionController;

    this->currentLongitudinalController->SetMaxSpeed(Maths::Abs(this->targetSpeed));
    this->currentLongitudinalController->DefineSetPoint(this->targetSpeed);
    this->currentAngularController->SetMaxSpeed(ANGULAR_MOVEMENT_SPEED_MAX);
    this->currentAngularController->DefineSetPoint(angularSetPoint);
}

bool GoThroughMovement::CheckAlmostDone(void)
{
    if (Maths::Distance(this->x, this->y, this->targetX, this->targetY) < 0.1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void GoThroughMovement::GoThrough(float x, float y, float speed)
{
    this->targetX     = x;
    this->targetY     = y;
    this->targetSpeed = speed;

     if (!TargetPointAhead(x, y, speed))
    {
        ChangeState(ST_MOVEMENT_INITIAL_ROTATION, 0, COMPLEX_MOVEMENT_STATE_MACHINE_TIMEOUT);
    }
    else
    {
        ChangeState(ST_MOVEMENT_ONGOING, 0, COMPLEX_MOVEMENT_STATE_MACHINE_TIMEOUT);
    }
}

/* RotationAroundPointMovement */

RotationAroundPointMovement::RotationAroundPointMovement(SpeedController * longitudinalSpeedController, PositionController * longitudinalPositionController,
                            SpeedController * angularSpeedController, PositionController * angularPositionController):
    ComplexMovement(longitudinalSpeedController, longitudinalPositionController, angularSpeedController, angularPositionController),
    centerX(0.0),
    centerY(0.0),
    direction(AngularMovement::CLOCKWISE),
    speed(0.0)
{
}

void RotationAroundPointMovement::ComputeInitialRotation(void)
{
    float angle;

    if (this->direction == AngularMovement::COUNTERCLOCKWISE)
    {
        angle = Maths::mod2pi(Maths::DeltaHeading(this->heading, this->x, this->y, x, y) - M_PI_2);
    }
    else
    {
        angle = Maths::mod2pi(Maths::DeltaHeading(this->heading, this->x, this->y, x, y) + M_PI_2);
    }

    this->currentLongitudinalController = this->longitudinalPositionController;
    this->currentAngularController = this->angularPositionController;

    this->currentLongitudinalController->SetMaxSpeed(LONGITUDINAL_MOVEMENT_SPEED_MAX);
    this->currentLongitudinalController->DefineSetPoint(this->longitudinalPosition);
    this->currentAngularController->SetMaxSpeed(ANGULAR_MOVEMENT_SPEED_MAX);
    this->currentAngularController->DefineSetPoint(this->angularPosition + angle);
}

void RotationAroundPointMovement::UpdateSetPoints(void)
{
    float deltaAngle, angle, targetX, targetY, angularSetPoint;

    deltaAngle = (this->longitudinalSpeed) / (2.0 * M_PI * this->radius) * 0.05;
    angle = atan2f(this->y - this->centerY, this->x - this->centerX);

    if (this->direction == AngularMovement::COUNTERCLOCKWISE)
    {
        targetX = this->centerX + this->radius * cosf(angle + deltaAngle);
        targetY = this->centerY + this->radius * sinf(angle + deltaAngle);
    }
    else
    {
        targetX = this->centerX + this->radius * cosf(angle - deltaAngle);
        targetY = this->centerY + this->radius * sinf(angle - deltaAngle);
    }

    angularSetPoint = this->heading + Maths::DeltaHeading(this->heading, this->x, this->y, targetX, targetY);

    this->currentLongitudinalController = this->longitudinalSpeedController;
    this->currentAngularController = this->angularPositionController;

    this->currentLongitudinalController->SetMaxSpeed(Maths::Abs(this->speed));
    this->currentLongitudinalController->DefineSetPoint(this->speed);
    this->currentAngularController->SetMaxSpeed(ANGULAR_MOVEMENT_SPEED_MAX);
    this->currentAngularController->DefineSetPoint(angularSetPoint);
}

bool RotationAroundPointMovement::CheckAlmostDone(void)
{
    return false;
}

void RotationAroundPointMovement::RotateAround(float x, float y, AngularMovement::Direction direction, float speed)
{
    this->centerX   = x;
    this->centerY   = y;
    this->direction = direction;
    this->speed     = speed;
    this->radius    = Maths::Distance(this->x, this->y, x, y);

    ChangeState(ST_MOVEMENT_INITIAL_ROTATION, 0, COMPLEX_MOVEMENT_STATE_MACHINE_TIMEOUT);
}


