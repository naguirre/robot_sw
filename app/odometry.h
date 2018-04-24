#ifndef __ODOMETRY_H__
#define __ODOMETRY_H__

/*************************************/
/* INCLUDES                          */
/*************************************/

#include <math.h>
#include <maths/maths.h>
#include <types.h>

#include <hal/encoder.h>

/*************************************/
/* DEFINES                           */
/*************************************/

#define ODOMETRY_AXIS_DISTANCE      0.223883
#define ODOMETRY_RIGHT_DIAMETER     0.0636288
#define ODOMETRY_LEFT_DIAMETER      0.0638029

#define ODOMETRY_ENCODER_STEPS      32
#define ODOMETRY_ENCODER_REDUCTION  1.0

/*************************************/
/* OBJET STRUCTURE                   */
/*************************************/

class Odometry
{

private:

    Encoder * encoders[2];

    struct{
        float period;
        float axisDistance;
        float rightDiameter;
        float leftDiameter;
    }settings;
    
    struct{
        struct{
            float rightPosition;
            float leftPosition;
            float rightSpeed;
            float leftSpeed;
        }rightLeft;
        struct{
            float longitudinalPosition;
            float angularPosition;
            float longitudinalSpeed;
            float angularSpeed;
        }polar;
        struct{
            float x;
            float y;
            float heading;
        }cartesian;
    }state;

public:

    Odometry(float period);

    ~Odometry(void);

    void  Run();

    float GetLongitudinalPosition();

    float GetAngularPosition();

    float GetLongitudinalSpeed();

    float GetAngularSpeed();

    float GetX();

    float GetY();

    float GetHeading();
};

#endif /* __ODOMETRIE_H__ */
