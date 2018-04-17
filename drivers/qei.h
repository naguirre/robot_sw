#ifndef Robot2_0_qei_h
#define Robot2_0_qei_h

#include <types.h>

/*************************************/
/* TYPES, TYPEDEFS, MACROS, DEFINES  */
/*************************************/

typedef enum{
    QEI0,
    QEI1,
    QEI2,
    QEI3,
}QEI_CHANNEL;

/*************************************/
/* OBJET STRUCTURE                   */
/*************************************/

typedef struct{
    QEI_CHANNEL channel;
}QEI;

/*************************************/
/* FONCTIONS                         */
/*************************************/

void QEI_Init(QEI * this, QEI_CHANNEL channel);

uint16_t QEI_GetNombrePas(QEI * this);

#endif
