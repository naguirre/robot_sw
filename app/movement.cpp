#include <app/movement.h>

Movement::Movement(SpeedController *longitudinalSpeedController, PositionController *longitudinalPositionController,
                   SpeedController *angularSpeedController, PositionController *angularPositionController):
    blockingCount(0),
    angularCommand(0.0),
    longitudinalCommand(0.0),
    longitudinalSpeedController(longitudinalSpeedController),
    longitudinalPositionController(longitudinalPositionController),
    currentLongitudinalController(longitudinalPositionController),
    angularSpeedController(angularSpeedController),
    angularPositionController(angularPositionController),
    currentAngularController(angularPositionController)
{
}

void Movement::Update(float longitudinalPosition, float longitudinalSpeed,
                      float angularPosition, float angularSpeed,
                      float x, float y, float heading)
{
    this->longitudinalPosition = longitudinalPosition;
    this->longitudinalSpeed    = longitudinalSpeed;
    this->angularPosition      = angularPosition;
    this->angularSpeed         = angularSpeed;
    this->x                    = x;
    this->y                    = y;
    this->heading              = heading;

    this->longitudinalCommand = this->currentLongitudinalController->Run(longitudinalSpeed, longitudinalPosition);
    this->angularCommand = this->currentAngularController->Run(angularSpeed, angularSpeed);
}

float Movement::GetAngularCommand()
{
    return this->angularCommand;
}

float Movement::GetLongitudinalCommand()
{
    return this->longitudinalCommand;
}