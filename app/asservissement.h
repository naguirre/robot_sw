#ifndef ASSERVISSEMENT_H
#define ASSERVISSEMENT_H

#include <types.h>

#include <app/pid.h>

/*************************************/
/* OBJET STRUCTURE                   */
/*************************************/

typedef enum{
    ASSERVISSEMENT_VITESSE,
    ASSERVISSEMENT_POSITION
}ASSERVISSEMENT_TYPE;


typedef struct
{
    struct{
        PID  pidPosition;
        PID  pidVitesse;
    }membres;
    struct{
        ASSERVISSEMENT_TYPE typeAsserv;
        float               valeur;
    }consigne;
    struct{
        float   periode;
        float   vitesseMax;
        float   accelerationMax;
    }parametres;
}ASSERVISSEMENT;


/*************************************/
/* FONCTIONS                         */
/*************************************/

void ASSERVISSEMENT_Init(ASSERVISSEMENT * this, float periode, float vitesseMax, float accelerationMax,
                         float commandeMaxVitesse,  float kpVitesse,  float kiVitesse,
                         float commandeMaxPosition, float kpPosition, float kiPosition);

void ASSERVISSEMENT_SetConsigne(ASSERVISSEMENT * this, ASSERVISSEMENT_TYPE type, float valeur);

void ASSERVISSEMENT_SetVitesseMax(ASSERVISSEMENT * this, float vitesseMax);

ASSERVISSEMENT_TYPE MOTEUR_ASSERVISSEMENT_GetConsigneType(ASSERVISSEMENT * this);

float ASSERVISSEMENT_GetConsigneValeur(ASSERVISSEMENT * this);

float ASSERVISSEMENT_Run(ASSERVISSEMENT * this, float mesureVitesse, float mesurePosition);

#endif
