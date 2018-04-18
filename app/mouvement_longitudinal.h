#ifndef __MOUVEMENT_LONGITUNDINAL_H__
#define __MOUVEMENT_LONGITUNDINAL_H__

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

#define MOUVEMENT_LONGITUDINAL_SEUIL_PRECISION  0.05   //5cm
#define MOUVEMENT_LONGITUDINAL_SEUIL_TERMINE    0.01   //2mm/s
#define MOUVEMENT_LONGITUDINAL_SEUIL_BLOCAGE    0.005
#define MOUVEMENT_LONGITUDINAL_MAX_BLOCAGE      10

#define MAE_MOUVEMENT_LONGITUDINAL_TIMEOUT      5000    //5 secondes

/*************************************/
/* OBJET STRUCTURE                   */
/*************************************/

class MOUVEMENT_LONGITUDINAL
{
    struct{
        DEPLACEMENT_CONSIGNE_TYPE       type;
        DEPLACEMENT_CONSIGNE_PARAMETRES parametres;
    }consigne;
    struct{
        MAE            *mae;
        ASSERVISSEMENT * asservissementLongitudinal;
        ASSERVISSEMENT * asservissementAngulaire;
    }membres;
    struct{
        uint8_t nbBlocages;
        DEPLACEMENT_MOUVEMENT_ETAT etatMouvement;
    }etat;

    MOUVEMENT_LONGITUDINAL(ASSERVISSEMENT * longitudinal, ASSERVISSEMENT * angulaire);

    void Run();

    void Avancer(float distance);

    void TranslationInfinie(float vitesse);

    DEPLACEMENT_MOUVEMENT_ETAT GetEtatDeplacement();
};

#endif /* __MOUVEMENT_LONGITUNDINAL_H__ */
