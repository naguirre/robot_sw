#ifndef MOUVEMENT_ANGULAIRE_H
#define MOUVEMENT_ANGULAIRE_H

/*************************************/
/* INCLUDES                          */
/*************************************/

#include <types.h>
#include <math.h>

#include <maths/maths.h>
#include <mae/mae.h>

#include <app/deplacement_types.h>
#include <app/asservissement.h>
#include <app/odometrie.h>
#include <app/mixer.h>
#include <app/interfaces.h>

/*************************************/
/* DEFINES                           */
/*************************************/

#define MOUVEMENT_ANGULAIRE_SEUIL_PRECISION     0.175   //10°
#define MOUVEMENT_ANGULAIRE_SEUIL_TERMINE       0.04    //0.5°/s
#define MOUVEMENT_ANGULAIRE_SEUIL_BLOCAGE       0.005
#define MOUVEMENT_ANGULAIRE_MAX_BLOCAGE         10

#define MAE_MOUVEMENT_ANGULAIRE_TIMEOUT         5000    //5 secondes

/*************************************/
/* OBJET STRUCTURE                   */
/*************************************/

typedef struct
{
    struct{
        DEPLACEMENT_CONSIGNE_TYPE       type;
        DEPLACEMENT_CONSIGNE_PARAMETRES parametres;
    }consigne;
    struct{
        MAE            mae;
        ASSERVISSEMENT * asservissementLongitudinal;
        ASSERVISSEMENT * asservissementAngulaire;
    }membres;
    struct{
        uint8_t nbBlocages;
        DEPLACEMENT_MOUVEMENT_ETAT etatMouvement;
    }etat;
}MOUVEMENT_ANGULAIRE;

/*************************************/
/* FONCTIONS                         */
/*************************************/

void MOUVEMENT_ANGULAIRE_Init(ASSERVISSEMENT * longitudinal, ASSERVISSEMENT * angulaire);

void MOUVEMENT_ANGULAIRE_Run();

DEPLACEMENT_MOUVEMENT_ETAT MOUVEMENT_ANGULAIRE_GetEtatDeplacement();

void MOUVEMENT_ANGULAIRE_Tourner(float angle);

void MOUVEMENT_ANGULAIRE_PointerVers(float x, float y);

void MOUVEMENT_ANGULAIRE_PointerDos(float x, float y);

void MOUVEMENT_ANGULAIRE_TangenterCercle(float x, float y, DEPLACEMENT_SENS_ROTATION sens);

void MOUVEMENT_ANGULAIRE_RotationInfinie(float vitesse);

#endif