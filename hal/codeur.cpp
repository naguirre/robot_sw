#include <hal/codeur.h>
#include <maths/maths.h>

void CODEUR_CalculerFacteurEchelle(CODEUR * this)
{
    this->parametres.facteurEchelle = 2.0 * PI / (4.0 * (float) (this->parametres.nombrePas) * this->parametres.reducteur);
}

void CODEUR_Init(CODEUR * this, uint8_t id, uint16_t nombrePasCodeur, float reducteur, float periode)
{
    this->etat.angleBrut = 0.0;
    this->etat.angleReel = 0.0;
    
    this->parametres.nombrePas = nombrePasCodeur;
    this->parametres.reducteur = reducteur;
    this->parametres.periode   = periode;
    
    QEI_Init(&this->qei, QEI_CHANNELS[id]);
    
    //Calcul de la distance représentée par un pas
    CODEUR_CalculerFacteurEchelle(this);
}

void CODEUR_ActualiserEtat(CODEUR * this)
{
    float angleBrutPrecedent = this->etat.angleBrut;
    
    this->etat.angleBrut = (float)QEI_GetNombrePas(&this->qei)*this->parametres.facteurEchelle;
    this->etat.angleReel = MATHS_mod2pi(this->etat.angleBrut);
    this->etat.vitesseAngulaire = (this->etat.angleBrut - angleBrutPrecedent) / this->parametres.periode;
}

float CODEUR_GetAngleBrut(CODEUR * this)
{
    return this->etat.angleBrut;
}

void CODEUR_SetAngleBrut(CODEUR * this, float Val)
{
    this->etat.angleBrut = Val;
    this->etat.vitesseAngulaire = 0.0;
}

float CODEUR_GetAngleReel(CODEUR * this)
{
    return this->etat.angleReel;
}

float CODEUR_GetVitesseAngulaire(CODEUR * this)
{
    return this->etat.vitesseAngulaire;
}
