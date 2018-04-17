#ifndef __QEI_H__
#define __QEI_H__

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

class QEI
{
    QEI_CHANNEL channel;

    QEI(QEI_CHANNEL channel);

    uint16_t GetNombrePas();
};

#endif /* __QEI_H__ */
