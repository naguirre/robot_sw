#include <app/movement.h>
#include <logs/logs.h>

Movement::Movement(SpeedController *longitudinalSpeedController, PositionController *longitudinalPositionController,
                   SpeedController *angularSpeedController, PositionController *angularPositionController):
    StateMachine(ST_MOVEMENT_STOP, ST_MOVEMENT_STOP, ST_MOVEMENT_MAX),
    blockingCount(0),
    angularCommand(0.0),
    longitudinalCommand(0.0),
    longitudinalSpeedController(longitudinalSpeedController),
    longitudinalPositionController(longitudinalPositionController),
    angularSpeedController(angularSpeedController),
    angularPositionController(angularPositionController),
    currentLongitudinalController(longitudinalPositionController),
    currentAngularController(angularPositionController)
{
}

void Movement::StateOnGoing()
{
    if (FirstTime())
    {
        INF("On Going");
        this->blockingCount = 0;
    }

    UpdateSetPoints();

    if (CheckBlocked())
    {
        ChangeState(ST_MOVEMENT_BLOCKED, 0, MOVEMENT_STATE_MACHINE_TIMEOUT);
    }

    if (CheckAlmostDone())
    {
        ChangeState(ST_MOVEMENT_ALMOST_DONE, 0, MOVEMENT_STATE_MACHINE_TIMEOUT);
    }
}

void Movement::StateAlmostDone(void)
{
    if (FirstTime())
    {
        INF("Almost done");
    }

    if (CheckDone())
    {
        ChangeState(ST_MOVEMENT_DONE, 0, MOVEMENT_STATE_MACHINE_TIMEOUT);
    }
}

void Movement::StateDone(void)
{
    if (FirstTime())
    {
        INF("Done");
    }
}

void Movement::StateBlocked(void)
{
    if (FirstTime())
    {
        INF("Blocked");
        Stop();
    }
}

void Movement::StateStop(void)
{
    if (FirstTime())
    {
        INF("Stop");
    }
}

void Movement::StateInitialRotation(void)
{
    if (FirstTime())
    {
        ComputeInitialRotation();
        this->blockingCount = 0;
        INF("Initial Rotation");
    }

    if (CheckRotationDone())
    {
        ChangeState(ST_MOVEMENT_ONGOING, 0, MOVEMENT_STATE_MACHINE_TIMEOUT);
    }
    else if (CheckRotationBlocked())
    {
        ChangeState(ST_MOVEMENT_BLOCKED, 0, 0);
    }
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
    this->angularCommand = this->currentAngularController->Run(angularSpeed, angularPosition);
}

float Movement::GetAngularCommand()
{
    return this->angularCommand;
}

float Movement::GetLongitudinalCommand()
{
    return this->longitudinalCommand;
}

void Movement::Stop(void)
{
    this->currentLongitudinalController = this->longitudinalPositionController;
    this->currentAngularController = this->angularPositionController;

    this->currentLongitudinalController->DefineSetPoint(this->longitudinalPosition);
    this->currentAngularController->DefineSetPoint(this->angularPosition);

    ChangeState(ST_MOVEMENT_STOP, 0, 0);
}

Movement::State Movement::GetState(void)
{
    return (Movement::State)(this->GetCurrentState());
}

void Movement::Run()
{
    Engine();
}