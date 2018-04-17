#ifndef MOUVEMENT_H
#define MOUVEMENT_H

/*************************************/
/* INCLUDES                          */
/*************************************/

#include <types.h>
#include <math.h>

#include <maths/maths.h>
#include <mae/mae.h>

#include <app/deplacement_types.h>
#include <app/mouvement_angulaire.h>
#include <app/mouvement_complexe.h>
#include <app/mouvement_longitudinal.h>
#include <app/asservissement.h>
#include <app/odometrie.h>
#include <app/mixer.h>
#include <app/interfaces.h>

/*************************************/
/* OBJET STRUCTURE                   */
/*************************************/

typedef enum
{
    DEPLACEMENT_MOUVEMENT_ARRET,
    DEPLACEMENT_MOUVEMENT_LONGITUDINAL,
    DEPLACEMENT_MOUVEMENT_ANGULAIRE,
    DEPLACEMENT_MOUVEMENT_COMPLEXE,
}DEPLACEMENT_MOUVEMENT_TYPE;

/*************************************/
/* OBJET STRUCTURE                   */
/*************************************/

typedef struct
{
    DEPLACEMENT_MOUVEMENT_TYPE typeMouvement;
    struct{
        MAE            mae;
        ASSERVISSEMENT asservissementLongitudinal;
        ASSERVISSEMENT asservissementAngulaire;
    }membres;
    struct{
        uint8_t nbBlocages;
        DEPLACEMENT_MOUVEMENT_ETAT etatMouvement;
    }etat;
}MOUVEMENT;

/*************************************/
/* FONCTIONS                         */
/*************************************/

void MOUVEMENT_Init(float periode);

void MOUVEMENT_Run();

void MOUVEMENT_ChangerConsigne(DEPLACEMENT_CONSIGNE_TYPE type, DEPLACEMENT_CONSIGNE_PARAMETRES * parametres);

DEPLACEMENT_MOUVEMENT_ETAT MOUVEMENT_GetEtatMouvement();

void MOUVEMENT_AllerA(float x, float y, float vitesse);

void MOUVEMENT_Avancer(float distance);

void MOUVEMENT_Tourner(float angle);

#endif
