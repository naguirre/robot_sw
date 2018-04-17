#ifndef __GPIO_H__
#define __GPIO_H__

/*************************************/
/* TYPES, TYPEDEFS, MACROS, DEFINES  */
/*************************************/

typedef enum{
    PORTA,
    PORTB,
    PORTC,
    PORTD,
    PORTF,
    PORTE,
}GPIO_PORT;

typedef unsigned char GPIO_PIN;

typedef enum{
    OUTPUT = 0,
    INPUT  = 1,
}GPIO_DIRECTION;

typedef enum{
    LOW  = 0,
    HIGH = 1,
}GPIO_ETAT;

/*************************************/
/* OBJET STRUCTURE                   */
/*************************************/

class GPIO
{
    GPIO_PORT port;
    GPIO_PIN  pin;
    GPIO_DIRECTION direction;
    GPIO_ETAT etat;

    GPIO(GPIO_PORT port, GPIO_PIN pin, GPIO_DIRECTION direction);

    GPIO_ETAT Read();

    void Write(GPIO_ETAT etat);

    void Toggle();
};

#endif /* __GPIO_H__ */
