#ifndef ODOMETRIE_H
#define ODOMETRIE_H

/*************************************/
/* INCLUDES                          */
/*************************************/

#include <math.h>
#include <maths/maths.h>
#include <types.h>

#include <app/interfaces.h>


/*************************************/
/* DEFINES                           */
/*************************************/

#define ODOMETRIE_ENTRE_AXE          0.223883
#define ODOMETRIE_DIAMETRE_DROITE    0.0636288
#define ODOMETRIE_DIAMETRE_GAUCHE    0.0638029

/*************************************/
/* OBJET STRUCTURE                   */
/*************************************/

typedef struct
{
    struct{
        float periode;
       	float entreAxe;
        float diametreDroite;
        float diametreGauche;
    }parametres;
	
    struct{
        struct{
            float positionDroite;
            float positionGauche;
            float vitesseDroite;
            float vitesseGauche;
        }droiteGauche;
        struct{
            float positionLongitudinale;
            float positionAngulaire;
            float vitesseLongitudinale;
            float vitesseAngulaire;
        }polaire;
        struct{
            float x;
            float y;
            float theta;
        }cartesien;
    }etat;
} ODOMETRIE;

/*************************************/
/* FONCTIONS                         */
/*************************************/

void ODOMETRIE_Init(float periode);

void ODOMETRIE_Run();

float ODOMETRIE_GetPositionLongitudinale();

float ODOMETRIE_GetPositionAngulaire();

float ODOMETRIE_GetVitesseLongitudinale();

float ODOMETRIE_GetVitesseAngulaire();

float ODOMETRIE_GetX();

float ODOMETRIE_GetY();

float ODOMETRIE_GetTheta();

#endif
