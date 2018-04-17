#ifndef INTERFACES_H
#define INTERFACES_H

/*************************************/
/* INCLUDES                          */
/*************************************/

#include <types.h>
#include <maths/maths.h>
#include <hal/codeur.h>
#include <hal/pont_h.h>


/*************************************/
/* TYPES                             */
/*************************************/

typedef enum{
    DEPLACEMENT_MOTEUR_DROITE,
    DEPLACEMENT_MOTEUR_GAUCHE,
    DEPLACEMENT_NB_MOTEURS,
}DEPLACEMENT_MOTEUR;


/*************************************/
/* OBJET STRUCTURE                   */
/*************************************/

typedef struct
{
    CODEUR codeurs[DEPLACEMENT_NB_MOTEURS];
    PONT_H pontsH[DEPLACEMENT_NB_MOTEURS];
}DEPLACEMENT_INTERFACES;

/*************************************/
/* FONCTIONS                         */
/*************************************/

void DEPLACEMENT_INTERFACES_Init(float periode);

void DEPLACEMENT_INTERFACES_ActualiserEtat();

float DEPLACEMENT_INTERFACES_GetAngleBrut(DEPLACEMENT_MOTEUR moteur);

void DEPLACEMENT_INTERFACES_SetAngleBrut(DEPLACEMENT_MOTEUR moteur, float angle);

float DEPLACEMENT_INTERFACES_GetVitesseAngulaire(DEPLACEMENT_MOTEUR moteur);

void DEPLACEMENT_INTERFACES_SetPuissance(DEPLACEMENT_MOTEUR moteur, float puissance);

#endif
