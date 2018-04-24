#include <app/odometry.h>
#include <app/robot.h>

Odometry::Odometry(float period)
{
    this->encoders[Robot::MOTOR_RIGHT] = new Encoder(Robot::MOTOR_RIGHT, ODOMETRY_ENCODER_STEPS, ODOMETRY_ENCODER_REDUCTION, period);
    this->encoders[Robot::MOTOR_LEFT]  = new Encoder(Robot::MOTOR_LEFT, ODOMETRY_ENCODER_STEPS, ODOMETRY_ENCODER_REDUCTION, period);

    this->simuMotors[Robot::MOTOR_RIGHT] = NULL;
    this->simuMotors[Robot::MOTOR_LEFT]  = NULL;

    this->settings.period         = period;
    this->settings.axisDistance   = ODOMETRY_AXIS_DISTANCE;
    this->settings.rightDiameter  = ODOMETRY_RIGHT_DIAMETER;
    this->settings.leftDiameter   = ODOMETRY_LEFT_DIAMETER;
    
    this->state.rightLeft.rightPosition = 0.0;
    this->state.rightLeft.leftPosition  = 0.0;
    this->state.rightLeft.rightSpeed    = 0.0;
    this->state.rightLeft.leftSpeed     = 0.0;
    
    this->state.polar.longitudinalPosition = 0.0;
    this->state.polar.angularPosition      = 0.0;
    this->state.polar.longitudinalSpeed    = 0.0;
    this->state.polar.angularSpeed         = 0.0;
    
    this->state.cartesian.x       = 0.0;
    this->state.cartesian.y       = 0.0;
    this->state.cartesian.heading = 0.0;
}

Odometry::Odometry(float period, SimuMotor * simuMotorRight, SimuMotor * simuMotorLeft)
{
    this->encoders[Robot::MOTOR_RIGHT] = NULL;
    this->encoders[Robot::MOTOR_LEFT]  = NULL;

    this->simuMotors[Robot::MOTOR_RIGHT] = simuMotorRight;
    this->simuMotors[Robot::MOTOR_LEFT]  = simuMotorLeft;

    this->settings.period         = period;
    this->settings.axisDistance   = ODOMETRY_AXIS_DISTANCE;
    this->settings.rightDiameter  = ODOMETRY_RIGHT_DIAMETER;
    this->settings.leftDiameter   = ODOMETRY_LEFT_DIAMETER;
    
    this->state.rightLeft.rightPosition = 0.0;
    this->state.rightLeft.leftPosition  = 0.0;
    this->state.rightLeft.rightSpeed    = 0.0;
    this->state.rightLeft.leftSpeed     = 0.0;
    
    this->state.polar.longitudinalPosition = 0.0;
    this->state.polar.angularPosition      = 0.0;
    this->state.polar.longitudinalSpeed    = 0.0;
    this->state.polar.angularSpeed         = 0.0;
    
    this->state.cartesian.x       = 0.0;
    this->state.cartesian.y       = 0.0;
    this->state.cartesian.heading = 0.0;
}

Odometry::~Odometry(void)
{
    delete this->encoders[Robot::MOTOR_RIGHT];
    delete this->encoders[Robot::MOTOR_LEFT];
}

void Odometry::Run(void)
{
    float newLongitudinalPosition;
    float newAngularPosition;
    float deltaLongitudinalDistance;

    if (this->encoders[Robot::MOTOR_RIGHT] && this->encoders[Robot::MOTOR_LEFT])
    {
        this->encoders[Robot::MOTOR_RIGHT]->Update();
        this->encoders[Robot::MOTOR_LEFT]->Update();

        this->state.rightLeft.rightPosition = this->encoders[Robot::MOTOR_RIGHT]->GetAngle() * this->settings.rightDiameter;
        this->state.rightLeft.leftPosition  = this->encoders[Robot::MOTOR_LEFT]->GetAngle()  * this->settings.leftDiameter;
        this->state.rightLeft.rightSpeed    = this->encoders[Robot::MOTOR_RIGHT]->GetSpeed() * this->settings.rightDiameter;
        this->state.rightLeft.leftSpeed     = this->encoders[Robot::MOTOR_LEFT]->GetSpeed()  * this->settings.leftDiameter;
    }
    else
    {
        this->simuMotors[Robot::MOTOR_RIGHT]->Run();
        this->simuMotors[Robot::MOTOR_LEFT]->Run();

        this->state.rightLeft.rightPosition = this->simuMotors[Robot::MOTOR_RIGHT]->GetAngle() * this->settings.rightDiameter;
        this->state.rightLeft.leftPosition  = this->simuMotors[Robot::MOTOR_LEFT]->GetAngle()  * this->settings.leftDiameter;
        this->state.rightLeft.rightSpeed    = this->simuMotors[Robot::MOTOR_RIGHT]->GetSpeed() * this->settings.rightDiameter;
        this->state.rightLeft.leftSpeed     = this->simuMotors[Robot::MOTOR_LEFT]->GetSpeed()  * this->settings.leftDiameter;
    }

    newLongitudinalPosition   = (this->state.rightLeft.rightPosition + this->state.rightLeft.leftPosition) / 2.0;
    newAngularPosition        = (this->state.rightLeft.rightPosition - this->state.rightLeft.leftPosition) / this->settings.axisDistance;
    deltaLongitudinalDistance = newLongitudinalPosition - this->state.polar.longitudinalPosition;
    
    this->state.polar.longitudinalSpeed    = deltaLongitudinalDistance / this->settings.period;
    this->state.polar.angularSpeed         = (newAngularPosition - this->state.polar.angularPosition) / (this->settings.period);
    this->state.polar.longitudinalPosition = newLongitudinalPosition;
    this->state.polar.angularPosition      = newAngularPosition;
    
    this->state.cartesian.heading = Maths::mod2pi(this->state.polar.angularPosition);
    this->state.cartesian.x += deltaLongitudinalDistance * cos(this->state.cartesian.heading);
    this->state.cartesian.y += deltaLongitudinalDistance * sin(this->state.cartesian.heading);
   
}

float Odometry::GetLongitudinalPosition()
{
    return this->state.polar.longitudinalPosition;
}

float Odometry::GetAngularPosition()
{
    return this->state.polar.angularPosition;
}

float Odometry::GetLongitudinalSpeed()
{
    return this->state.polar.longitudinalSpeed;
}

float Odometry::GetAngularSpeed()
{
    return this->state.polar.angularSpeed;
}

float Odometry::GetX()
{
    return this->state.cartesian.x;
}

float Odometry::GetY()
{
    return this->state.cartesian.y;
}

float Odometry::GetHeading()
{
    return this->state.cartesian.heading;
}
