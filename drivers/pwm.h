#ifndef Robot2_0_pwm_h
#define Robot2_0_pwm_h

/*************************************/
/* TYPES, TYPEDEFS, MACROS, DEFINES  */
/*************************************/

typedef enum{
    PWM0,
    PWM1,
    PWM2,
    PWM3,
}PWM_CHANNEL;

/*************************************/
/* OBJET STRUCTURE                   */
/*************************************/

typedef struct{
    PWM_CHANNEL channel;
    float       rapportCyclique;
}PWM;

/*************************************/
/* FONCTIONS                         */
/*************************************/

void PWM_Init(PWM * this, PWM_CHANNEL channel);

void PWM_ChangerRapportCyclique(PWM * this, float rapport);

#endif
