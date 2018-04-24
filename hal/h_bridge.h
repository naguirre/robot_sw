#ifndef Robot2_0_pont_h_h
#define Robot2_0_pont_h_h

#include <drivers/pwm.h>
#include <drivers/gpio.h>

#include <types.h>
#include <maths/maths.h>

/*************************************/
/* OBJET STRUCTURE                   */
/*************************************/

class HBridge{

private:

    Pwm  * pwm;
    Gpio * gpio;

    //Module utilisé pour chaque codeur
    const Pwm::Channel PWM_CHANNELS[2] = {Pwm::CHANNEL0, Pwm::CHANNEL1};
    const Gpio::Port   SENS_PORTS[2]   = {Gpio::PORTB, Gpio::PORTB};
    const Gpio::Pin    SENS_PINS[2]    = {0, 2};

public:

    HBridge(uint8_t idMoteur);

    ~HBridge(void);

    /* Set command between -1.0 and 1.0 */
    void SetCommand(float puissance);
};


#endif
