#include <hal/pont_h.h>

void PONT_H_Init(PONT_H * this, uint8_t idMoteur)
{
    PWM_Init(&this->pwm, PWM_CHANNELS[idMoteur]);
    GPIO_Init(&this->gpio, SENS_PORTS[idMoteur], SENS_PINS[idMoteur], OUTPUT);
    
    PWM_ChangerRapportCyclique(&this->pwm, 0.0);
    GPIO_Write(&this->gpio, LOW);
}

void PONT_H_ChangerPuissance(PONT_H * this, float puissance)
{
    PWM_ChangerRapportCyclique(&this->pwm, MATHS_abs(puissance));
    GPIO_Write(&this->gpio, (GPIO_ETAT)MATHS_estPositif(puissance));
}
