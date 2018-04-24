#include <app/mixer.h>
#include <app/robot.h>

Mixer::Mixer(void)
{
    this->hBridges[Robot::MOTOR_RIGHT] = new HBridge(Robot::MOTOR_RIGHT);
    this->hBridges[Robot::MOTOR_LEFT] = new HBridge(Robot::MOTOR_LEFT);

    this->simuMotors[Robot::MOTOR_RIGHT] = NULL;
    this->simuMotors[Robot::MOTOR_LEFT]  = NULL;
}

Mixer::Mixer(SimuMotor * simuMotorRight, SimuMotor * simuMotorLeft)
{
    this->simuMotors[Robot::MOTOR_RIGHT] = simuMotorRight;
    this->simuMotors[Robot::MOTOR_LEFT]  = simuMotorLeft;

    this->hBridges[Robot::MOTOR_RIGHT] = NULL;
    this->hBridges[Robot::MOTOR_LEFT]  = NULL;
}

Mixer::~Mixer(void)
{
    if (this->hBridges[Robot::MOTOR_RIGHT])
    {
        delete this->hBridges[Robot::MOTOR_RIGHT];
    }
    
    if (this->hBridges[Robot::MOTOR_LEFT])
    {
        delete this->hBridges[Robot::MOTOR_LEFT];
    }
}

void Mixer::ApplyRightCommand(float command)
{
    command = Maths::Limit(command, 1.0);

    if (this->hBridges[Robot::MOTOR_RIGHT])
    {
        this->hBridges[Robot::MOTOR_RIGHT]->SetCommand(command);
    }
    else
    {
        this->simuMotors[Robot::MOTOR_RIGHT]->SetCommand(command);
    }
    
}

void Mixer::ApplyLeftCommand(float command)
{
    command = Maths::Limit(command, 1.0);

    if (this->hBridges[Robot::MOTOR_LEFT])
    {
        this->hBridges[Robot::MOTOR_LEFT]->SetCommand(command);
    }
    else
    {
        this->simuMotors[Robot::MOTOR_LEFT]->SetCommand(command);
    }
}

void Mixer::ApplyRightLeftCommand(float rightCommand, float leftCommand)
{
    ApplyRightCommand(rightCommand);
    ApplyLeftCommand(leftCommand);
}

void Mixer::ApplyPolarCommand(float longitudinalCommand, float angularCommand)
{
    ApplyRightCommand(longitudinalCommand + angularCommand);
    ApplyLeftCommand(longitudinalCommand - angularCommand);
}
