#ifndef Robot2_0_gpio_h
#define Robot2_0_gpio_h

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

typedef struct{
    GPIO_PORT port;
    GPIO_PIN  pin;
    GPIO_DIRECTION direction;
    GPIO_ETAT etat;
}GPIO;

/*************************************/
/* FONCTIONS                         */
/*************************************/

void GPIO_Init(GPIO * this, GPIO_PORT port, GPIO_PIN pin, GPIO_DIRECTION direction);

GPIO_ETAT GPIO_Read(GPIO * this);

void GPIO_Write(GPIO * this, GPIO_ETAT etat);

void GPIO_Toggle(GPIO * this);

#endif
