#include <hal/pont_h.h>

void PONT_H::PONT_H(uint8_t idMoteur)
{
    pwm = PWM(PWM_CHANNELS[idMoteur]);
    gpio = GPIO(SENS_PORTS[idMoteur], SENS_PINS[idMoteur], OUTPUT);

    pwm->ChangerRapportCyclique(0.0);
    gpio->Write(LOW);
}

void PONT_H::ChangerPuissance(float puissance)
{
    pwm->ChangerRapportCyclique(MATHS_abs(puissance));
    gpio->Write((GPIO_ETAT)MATHS_estPositif(puissance));
}
