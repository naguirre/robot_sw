#ifndef Robot2_0_pont_h_h
#define Robot2_0_pont_h_h

#include <drivers/pwm.h>
#include <drivers/gpio.h>

#include <types.h>
#include <maths/maths.h>

/*************************************/
/* HARDWARE MAPPING                  */
/*************************************/

//Module utilisé pour chaque codeur
static const PWM_CHANNEL PWM_CHANNELS[4] = {PWM0, PWM1, PWM2, PWM3};
static const GPIO_PORT   SENS_PORTS[4]   = {PORTB, PORTB, PORTB, PORTB};
static const uint8_t     SENS_PINS[4]    = {0, 2, 4, 6};


/*************************************/
/* OBJET STRUCTURE                   */
/*************************************/

typedef struct{
    PWM         pwm;
    GPIO        gpio;
}PONT_H;

typedef enum{
    SENS_ARRIERE = 0,
    SENS_AVANT = 1,
}PONT_H_SENS;

/*************************************/
/* FONCTIONS                         */
/*************************************/

void PONT_H_Init(PONT_H * this, uint8_t idMoteur);

void PONT_H_ChangerPuissance(PONT_H * this, float puissance);


#endif
