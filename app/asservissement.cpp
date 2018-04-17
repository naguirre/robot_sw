#include <app/asservissement.h>
#include <stdio.h>

void ASSERVISSEMENT_Init(ASSERVISSEMENT * this, float periode, float vitesseMax, float accelerationMax,
                         float commandeMaxVitesse,  float kpVitesse,  float kiVitesse,
                         float commandeMaxPosition, float kpPosition, float kiPosition)
{
    PID_Init(&this->membres.pidPosition, periode, kpVitesse, kiVitesse, 0.0, commandeMaxPosition);

    PID_Init(&this->membres.pidVitesse, periode, kpVitesse, kiVitesse, 0.0, commandeMaxVitesse);
    
    this->parametres.periode         = periode;
    this->parametres.vitesseMax      = vitesseMax;
    this->parametres.accelerationMax = accelerationMax;
}

float ASSERVISSEMENT_Vitesse(ASSERVISSEMENT * this, float mesureVitesse)
{
    //On récupère la consigne de vitesse précédente pour limiter l'accélération
    float consigneVitessePrecedente = PID_GetConsigne(&this->membres.pidVitesse);
    //La consigne de vitesse est la consigne de l'asservissement global
    float consigneVitesse = this->consigne.valeur;
    //La consigne de vitesse est saturée et de dérivée (accélération) saturée
    consigneVitesse = MATHS_Saturer(consigneVitesse, this->parametres.vitesseMax);
    consigneVitesse = MATHS_SaturerDerivee(consigneVitesse, consigneVitessePrecedente, this->parametres.periode,
                                           this->parametres.accelerationMax);
    
    return PID_Run(&this->membres.pidVitesse, consigneVitesse, mesureVitesse);
}


float ASSERVISSEMENT_Position(ASSERVISSEMENT * this, float mesureVitesse, float mesurePosition)
{
    //La consigne de position est la consigne de l'asservissement global
    float consignePosition = this->consigne.valeur;
    //On récupère la consigne de vitesse précédente pour limiter l'accélération
    float consigneVitessePrecedente = PID_GetConsigne(&this->membres.pidVitesse);
    //La consigne de vitesse est la commande du PID de position
    float consigneVitesse = this->parametres.vitesseMax*PID_Run(&this->membres.pidPosition, consignePosition, mesurePosition);
    //La consigne de vitesse est saturée et de dérivée (accélération) saturée
    consigneVitesse = MATHS_Saturer(consigneVitesse, this->parametres.vitesseMax);
    consigneVitesse = MATHS_SaturerDerivee(consigneVitesse, consigneVitessePrecedente, this->parametres.periode,
                                           this->parametres.accelerationMax);
    
    return PID_Run(&this->membres.pidVitesse, consigneVitesse, mesureVitesse);
}

void ASSERVISSEMENT_SetVitesseMax(ASSERVISSEMENT * this, float vitesseMax)
{
    this->parametres.vitesseMax = vitesseMax;
}

void ASSERVISSEMENT_SetConsigne(ASSERVISSEMENT * this, ASSERVISSEMENT_TYPE type, float valeur)
{
    this->consigne.typeAsserv = type;
    this->consigne.valeur = valeur;
}

float ASSERVISSEMENT_GetConsigneValeur(ASSERVISSEMENT * this)
{
    return this->consigne.valeur;
}

float ASSERVISSEMENT_Run(ASSERVISSEMENT * this, float mesureVitesse, float mesurePosition)
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