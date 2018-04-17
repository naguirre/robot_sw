#include "app/asservissement.h"
#include <iostream>

ASSERVISSEMENT::ASSERVISSEMENT(loat periode, float vitesseMax, float accelerationMax,
                         float commandeMaxVitesse,  float kpVitesse,  float kiVitesse,
                         float commandeMaxPosition, float kpPosition, float kiPosition)
{
    PID_Init(&membres.pidPosition, periode, kpVitesse, kiVitesse, 0.0, commandeMaxPosition);

    PID_Init(&membres.pidVitesse, periode, kpVitesse, kiVitesse, 0.0, commandeMaxVitesse);

    parametres.periode         = periode;
    parametres.vitesseMax      = vitesseMax;
    parametres.accelerationMax = accelerationMax;
}

ASSERVISSEMENT::~ASSERVISSEMENT()
{

}

float ASSERVISSEMENT::Vitesse(float mesureVitesse)
{
    //On récupère la consigne de vitesse précédente pour limiter l'accélération
    float consigneVitessePrecedente = PID_GetConsigne(&membres.pidVitesse);
    //La consigne de vitesse est la consigne de l'asservissement global
    float consigneVitesse = consigne.valeur;
    //La consigne de vitesse est saturée et de dérivée (accélération) saturée
    consigneVitesse = MATHS_Saturer(consigneVitesse, parametres.vitesseMax);
    consigneVitesse = MATHS_SaturerDerivee(consigneVitesse, consigneVitessePrecedente, parametres.periode,
                                           parametres.accelerationMax);

    return PID_Run(&membres.pidVitesse, consigneVitesse, mesureVitesse);
}


float ASSERVISSEMENT::Position(float mesureVitesse, float mesurePosition)
{
    //La consigne de position est la consigne de l'asservissement global
    float consignePosition = consigne.valeur;
    //On récupère la consigne de vitesse précédente pour limiter l'accélération
    float consigneVitessePrecedente = PID_GetConsigne(&membres.pidVitesse);
    //La consigne de vitesse est la commande du PID de position
    float consigneVitesse = parametres.vitesseMax*PID_Run(&membres.pidPosition, consignePosition, mesurePosition);
    //La consigne de vitesse est saturée et de dérivée (accélération) saturée
    consigneVitesse = MATHS_Saturer(consigneVitesse, parametres.vitesseMax);
    consigneVitesse = MATHS_SaturerDerivee(consigneVitesse, consigneVitessePrecedente, parametres.periode,
                                           parametres.accelerationMax);

    return PID_Run(&membres.pidVitesse, consigneVitesse, mesureVitesse);
}

void ASSERVISSEMENT::SetVitesseMax(float vitesseMax)
{
    parametres.vitesseMax = vitesseMax;
}

void ASSERVISSEMENT::SetConsigne(ASSERVISSEMENT_TYPE type, float valeur)
{
    consigne.typeAsserv = type;
    consigne.valeur = valeur;
}

float ASSERVISSEMENT::GetConsigneValeur()
{
    return consigne.valeur;
}

float ASSERVISSEMENT::Run(float mesureVitesse, float mesurePosition)
{
    if(this->consigne.typeAsserv == ASSERVISSEMENT_POSITION)
    {
        return ASSERVISSEMENT_Position(this, mesureVitesse, mesurePosition);
    }
    else if(this->consigne.typeAsserv == ASSERVISSEMENT_VITESSE)
    {
        return ASSERVISSEMENT_Vitesse(this, mesureVitesse);
    }

    //Défaut
    return 0.0;
}
