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


class ASSERVISSEMENT
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

    ASSERVISSEMENT(float periode, float vitesseMax, float accelerationMax,
                             float commandeMaxVitesse,  float kpVitesse,  float kiVitesse,
                             float commandeMaxPosition, float kpPosition, float kiPosition);
    ~ASSERVISSEMENT();

    void SetConsigne(ASSERVISSEMENT_TYPE type, float valeur);

    void SetVitesseMax(float vitesseMax);

    ASSERVISSEMENT_TYPE GetConsigneType();

    float GetConsigneValeur();

    float Run(float mesureVitesse, float mesurePosition);
};

#endif
