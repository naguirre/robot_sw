#ifndef __ENCODER_H__
#define __ENCODER_H__

#include <types.h>

#include <drivers/qei.h>


class Encoder
{

private:

    Qei * qei;   // QEI peripheral abstraction
    
    struct{
        float    reduction;     // Reduction between motor output axis and encodeur axis
        uint16_t stepsCount;    // Number of encodeur steps
        float    scaleFactor;   // Distance driven by a wheel for one encodeur step
        float    period;        // sampling period
    }settings;
    
    struct{
        float    angle;         // absolute angle (without mod_2_pi) driven by wheel (rad)
        float    speed;         // (rad/s)
    }state;

    // Hardware mapping
    const Qei::Channel QEI_CHANNELS[2] = {Qei::CHANNEL0, Qei::CHANNEL1};

    void ComputeScaleFactor();

public:

    Encoder(uint8_t idMotor, uint16_t stepsCount, float reduction, float period);

    ~Encoder(void);

    void Update();

    float GetAngle();

    void  SetAngle(float value);

    float GetSpeed();
};

#endif /* __ENCODER_H__ */
