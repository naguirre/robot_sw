#ifndef __INTERFACES_H__
#define __INTERFACES_H__

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

class DEPLACEMENT_INTERFACES
{
public:
    CODEUR *codeurs[DEPLACEMENT_NB_MOTEURS];
    PONT_H *pontsH[DEPLACEMENT_NB_MOTEURS];

    DEPLACEMENT_INTERFACES(float periode);

    void ActualiserEtat();

    float GetAngleBrut(DEPLACEMENT_MOTEUR moteur);

    void SetAngleBrut(DEPLACEMENT_MOTEUR moteur, float angle);

    float GetVitesseAngulaire(DEPLACEMENT_MOTEUR moteur);

    void SetPuissance(DEPLACEMENT_MOTEUR moteur, float puissance);
};

#endif /* __INTERFACES_H__ */
