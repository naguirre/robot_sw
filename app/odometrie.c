#include <app/odometrie.h>

ODOMETRIE odometrie;
static ODOMETRIE * this = &odometrie;

void ODOMETRIE_Init(float periode)
{
    DEPLACEMENT_INTERFACES_Init(periode);
    
    this->parametres.periode        = periode;
    this->parametres.entreAxe       = ODOMETRIE_ENTRE_AXE;
    this->parametres.diametreDroite = ODOMETRIE_DIAMETRE_DROITE;
    this->parametres.diametreGauche = ODOMETRIE_DIAMETRE_GAUCHE;
    
    this->etat.droiteGauche.positionDroite = 0.0;
    this->etat.droiteGauche.positionGauche = 0.0;
    this->etat.droiteGauche.vitesseDroite  = 0.0;
    this->etat.droiteGauche.vitesseGauche  = 0.0;
    
    this->etat.polaire.positionLongitudinale = 0.0;
    this->etat.polaire.positionAngulaire     = 0.0;
    this->etat.polaire.vitesseLongitudinale  = 0.0;
    this->etat.polaire.vitesseAngulaire      = 0.0;
    
    this->etat.cartesien.x      = -1.5;
    this->etat.cartesien.y      = 1.0;
    this->etat.cartesien.theta  = 0.0;
}

void ODOMETRIE_Run()
{
    float nouvellePositionLongitudinale;
    float nouvellePositionAngulaire;
    float deltaDistanceLongitudinale;
    
    DEPLACEMENT_INTERFACES_ActualiserEtat();
    
    this->etat.droiteGauche.positionDroite = DEPLACEMENT_INTERFACES_GetAngleBrut(DEPLACEMENT_MOTEUR_DROITE) * this->parametres.diametreDroite;
    this->etat.droiteGauche.positionGauche = DEPLACEMENT_INTERFACES_GetAngleBrut(DEPLACEMENT_MOTEUR_GAUCHE) * this->parametres.diametreGauche;
    this->etat.droiteGauche.vitesseDroite  = DEPLACEMENT_INTERFACES_GetVitesseAngulaire(DEPLACEMENT_MOTEUR_DROITE) * this->parametres.diametreDroite;
    this->etat.droiteGauche.vitesseGauche  = DEPLACEMENT_INTERFACES_GetVitesseAngulaire(DEPLACEMENT_MOTEUR_GAUCHE) * this->parametres.diametreGauche;
    
    nouvellePositionLongitudinale = (this->etat.droiteGauche.positionDroite + this->etat.droiteGauche.positionGauche) / 2.0;
    nouvellePositionAngulaire = (this->etat.droiteGauche.positionDroite - this->etat.droiteGauche.positionGauche) / this->parametres.entreAxe;
    deltaDistanceLongitudinale = nouvellePositionLongitudinale - this->etat.polaire.positionLongitudinale;
    this->etat.polaire.vitesseLongitudinale = deltaDistanceLongitudinale / this->parametres.periode;
    this->etat.polaire.vitesseAngulaire = (nouvellePositionAngulaire - this->etat.polaire.positionAngulaire) / (this->parametres.periode);
    this->etat.polaire.positionLongitudinale = nouvellePositionLongitudinale;
    this->etat.polaire.positionAngulaire     = nouvellePositionAngulaire;
    
    this->etat.cartesien.theta = MATHS_mod2pi(this->etat.polaire.positionAngulaire);
    this->etat.cartesien.x += deltaDistanceLongitudinale * cos(this->etat.cartesien.theta);
    this->etat.cartesien.y += deltaDistanceLongitudinale * sin(this->etat.cartesien.theta);
    
}

float ODOMETRIE_GetPositionLongitudinale()
{
    return this->etat.polaire.positionLongitudinale;
}

float ODOMETRIE_GetPositionAngulaire()
{
    return this->etat.polaire.positionAngulaire;
}

float ODOMETRIE_GetVitesseLongitudinale()
{
    return this->etat.polaire.vitesseLongitudinale;
}

float ODOMETRIE_GetVitesseAngulaire()
{
    return this->etat.polaire.vitesseAngulaire;
}

float ODOMETRIE_GetX()
{
    return this->etat.cartesien.x;
}

float ODOMETRIE_GetY()
{
    return this->etat.cartesien.y;
}

float ODOMETRIE_GetTheta()
{
    return this->etat.cartesien.theta;
}
