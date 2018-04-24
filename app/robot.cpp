#include <app/robot.h>
#include <logs/logs.h>

#define LONGITUNAL_MAX_SPEED            1.2
#define LONGITUNAL_MAX_ACCELERATION     4.0

#define LONGITUNAL_SPEED_MAX            400.0
#define LONGITUNAL_SPEED_KP             220.0
#define LONGITUNAL_SPEED_KI             0.0

#define LONGITUNAL_DISTANCE_MAX         400.0
#define LONGITUNAL_DISTANCE_KP          2200.0
#define LONGITUNAL_DISTANCE_KI          0.0

#define ANGULAR_MAX_SPEED               8.0
#define ANGULAR_MAX_ACCELERATION        40.0

#define ANGULAR_SPEED_MAX               400.0
#define ANGULAR_SPEED_KP                18.0
#define ANGULAR_SPEED_KI                0.0

#define ANGULAR_DISTANCE_MAX            600.0
#define ANGULAR_DISTANCE_KP             480.0
#define ANGULAR_DISTANCE_KI             0.0

Robot::Robot(float period, bool simu)
{
    if (simu)
    {
        this->simuMotors[MOTOR_RIGHT] = new SimuMotor(period);
        this->simuMotors[MOTOR_LEFT]  = new SimuMotor(period);

        this->odometry = new Odometry(period, this->simuMotors[MOTOR_RIGHT], this->simuMotors[MOTOR_LEFT]);
        this->mixer    = new Mixer(this->simuMotors[MOTOR_RIGHT], this->simuMotors[MOTOR_LEFT]);
    }
    else
    {
        this->odometry = new Odometry(period);
        this->mixer    = new Mixer();
    }

    this->longitudinalPositionController = new PositionController(period, LONGITUNAL_MAX_SPEED, LONGITUNAL_MAX_ACCELERATION,
                                                                  LONGITUNAL_SPEED_MAX, LONGITUNAL_SPEED_KP, LONGITUNAL_SPEED_KI,
                                                                  LONGITUNAL_DISTANCE_MAX, LONGITUNAL_DISTANCE_KP, LONGITUNAL_DISTANCE_KI);

    this->angularPositionController = new PositionController(period, ANGULAR_MAX_SPEED, ANGULAR_MAX_ACCELERATION,
                                                             ANGULAR_SPEED_MAX, ANGULAR_SPEED_KP, ANGULAR_SPEED_KI,
                                                             ANGULAR_DISTANCE_MAX, ANGULAR_DISTANCE_KP, ANGULAR_DISTANCE_KI);

    this->longitudinalSpeedController = new SpeedController(period, LONGITUNAL_MAX_SPEED, LONGITUNAL_MAX_ACCELERATION,
                                                            LONGITUNAL_SPEED_MAX, LONGITUNAL_SPEED_KP, LONGITUNAL_SPEED_KI);

    this->angularSpeedController = new SpeedController(period, ANGULAR_MAX_SPEED, ANGULAR_MAX_ACCELERATION,
                                                       ANGULAR_SPEED_MAX, ANGULAR_SPEED_KP, ANGULAR_SPEED_KI);

    this->angularMovement = new AngularMovement(this->longitudinalSpeedController, this->longitudinalPositionController,
                                                this->angularSpeedController, this->angularPositionController);

    this->longitudinalMovement = new LongitudinalMovement(this->longitudinalSpeedController, this->longitudinalPositionController,
                                                          this->angularSpeedController, this->angularPositionController);

    this->Stop();
}

void Robot::Run(void)
{
    float longitudinalCommand;
    float angularCommand;

    this->odometry->Run();

    this->currentMovement->Update(this->odometry->GetLongitudinalPosition(), this->odometry->GetLongitudinalSpeed(),
                                  this->odometry->GetAngularPosition(), this->odometry->GetAngularSpeed(),
                                  this->odometry->GetX(), this->odometry->GetY(), this->odometry->GetHeading());

    this->currentMovement->Run();

    longitudinalCommand = this->currentMovement->GetLongitudinalCommand();
    angularCommand      = this->currentMovement->GetAngularCommand();

    this->mixer->ApplyPolarCommand(longitudinalCommand, angularCommand);
}

void Robot::Translate(float distance)
{
    this->longitudinalMovement->Update(this->odometry->GetLongitudinalPosition(), this->odometry->GetLongitudinalSpeed(),
                                       this->odometry->GetAngularPosition(), this->odometry->GetAngularSpeed(),
                                       this->odometry->GetX(), this->odometry->GetY(), this->odometry->GetHeading());

    this->longitudinalMovement->Translate(distance);

    this->currentMovement = this->longitudinalMovement;
}

void Robot::Rotate(float angle)
{
    this->angularMovement->Update(this->odometry->GetLongitudinalPosition(), this->odometry->GetLongitudinalSpeed(),
                                  this->odometry->GetAngularPosition(), this->odometry->GetAngularSpeed(),
                                  this->odometry->GetX(), this->odometry->GetY(), this->odometry->GetHeading());

    this->angularMovement->Rotate(angle);

    this->currentMovement = this->angularMovement;
}

void Robot::Stop(void)
{
    this->longitudinalMovement->Update(this->odometry->GetLongitudinalPosition(), this->odometry->GetLongitudinalSpeed(),
                                       this->odometry->GetAngularPosition(), this->odometry->GetAngularSpeed(),
                                       this->odometry->GetX(), this->odometry->GetY(), this->odometry->GetHeading());

    this->longitudinalMovement->Stop();

    this->currentMovement = this->longitudinalMovement;
}

bool Robot::CheckMovementOngoing()
{
    return (this->currentMovement->GetState() == Movement::ST_MOVEMENT_ONGOING);
}

bool Robot::CheckMovementDone()
{
    return (this->currentMovement->GetState() == Movement::ST_MOVEMENT_DONE);
}

bool Robot::CheckMovementBlocked()
{
    return (this->currentMovement->GetState() == Movement::ST_MOVEMENT_BLOCKED);
}



