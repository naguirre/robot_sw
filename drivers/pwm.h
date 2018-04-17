#ifndef __PWM_H__
#define __PWM_H__

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

class PWM
{
    PWM_CHANNEL channel;
    float       rapportCyclique;

    PWM(PWM_CHANNEL channel);

    void ChangerRapportCyclique(float rapport);
};

#endif /* __PWM_H__ */
