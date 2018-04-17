#include <app/mouvement.h>
#include <stdio.h>

MOUVEMENT mouvement;
static MOUVEMENT * this = &mouvement;

void MOUVEMENT_Stop();

void MOUVEMENT_Init(float periode)
{
    //Initialisation des membres
    MOUVEMENT_LONGITUDINAL_Init(&this->membres.asservissementLongitudinal, &this->membres.asservissementAngulaire);
    MOUVEMENT_ANGULAIRE_Init(&this->membres.asservissementLongitudinal, &this->membres.asservissementAngulaire);
    MOUVEMENT_COMPLEXE_Init(&this->membres.asservissementLongitudinal, &this->membres.asservissementAngulaire);
    ASSERVISSEMENT_Init(&this->membres.asservissementLongitudinal, periode, 1.2, 4.0,  400.0, 2200.0, 0.0, 400.0, 220.0, 0.0);
    ASSERVISSEMENT_Init(&this->membres.asservissementAngulaire,    periode, 8.0, 40.0, 600.0, 480.0, 0.0, 400.0, 18.0, 0.0);
    ODOMETRIE_Init(periode);
    
    MOUVEMENT_Stop();
    this->etat.etatMouvement = DEPLACEMENT_MOUVEMENT_STOP;
    this->etat.nbBlocages    = 0;
    
    //Initialisation de la consigne
    ASSERVISSEMENT_SetConsigne(&this->membres.asservissementLongitudinal, ASSERVISSEMENT_POSITION, 0.0);
    ASSERVISSEMENT_SetConsigne(&this->membres.asservissementAngulaire,    ASSERVISSEMENT_POSITION, 0.0);
    
}

/******** ENTREES / SORTIES ***********/

void MOUVEMENT_ActualiserEtat()
{
    ODOMETRIE_Run();
    
    if(this->typeMouvement == DEPLACEMENT_MOUVEMENT_ANGULAIRE)
    {
        this->etat.etatMouvement = MOUVEMENT_ANGULAIRE_GetEtatDeplacement();
    }
    else if(this->typeMouvement == DEPLACEMENT_MOUVEMENT_LONGITUDINAL)
    {
        this->etat.etatMouvement = MOUVEMENT_LONGITUDINAL_GetEtatDeplacement();
    }
    else if (this->typeMouvement == DEPLACEMENT_MOUVEMENT_COMPLEXE)
    {
        this->etat.etatMouvement = MOUVEMENT_COMPLEXE_GetEtatDeplacement();
    }
    else if (this->typeMouvement == DEPLACEMENT_MOUVEMENT_ARRET)
    {
        this->etat.etatMouvement = DEPLACEMENT_MOUVEMENT_STOP;
    }
}

void MOUVEMENT_CalculerEtAppliquerCommande()
{
    float commandeLongitudinale;
    float commandeAngulaire;
    
    //printf("%f; %f\n", ODOMETRIE_GetX(), ODOMETRIE_GetY());
    commandeLongitudinale = ASSERVISSEMENT_Run(&this->membres.asservissementLongitudinal, ODOMETRIE_GetVitesseLongitudinale(),
                                               ODOMETRIE_GetPositionLongitudinale());
    commandeAngulaire     = ASSERVISSEMENT_Run(&this->membres.asservissementAngulaire, ODOMETRIE_GetVitesseAngulaire(),
                                               ODOMETRIE_GetPositionAngulaire());
    
    MIXER_AppliquerCommandePolaire(commandeLongitudinale, commandeAngulaire);
}

/******** CALCULS DE CONSIGNES ***********/

void MOUVEMENT_Stop()
{
    float positionLongitudinale = ODOMETRIE_GetPositionLongitudinale();
    float positionAngulaire     = ODOMETRIE_GetPositionAngulaire();
    
    this->typeMouvement = DEPLACEMENT_MOUVEMENT_ARRET;
    ASSERVISSEMENT_SetConsigne(&this->membres.asservissementLongitudinal, ASSERVISSEMENT_POSITION, positionLongitudinale);
    ASSERVISSEMENT_SetConsigne(&this->membres.asservissementAngulaire, ASSERVISSEMENT_POSITION, positionAngulaire);
}

void MOUVEMENT_Avancer(float distance)
{
    this->typeMouvement = DEPLACEMENT_MOUVEMENT_LONGITUDINAL;
    MOUVEMENT_LONGITUDINAL_Avancer(distance);
}

void MOUVEMENT_TranslationInfinie(float vitesse)
{
    this->typeMouvement = DEPLACEMENT_MOUVEMENT_LONGITUDINAL;
    MOUVEMENT_LONGITUDINAL_TranslationInfinie(vitesse);
}

void MOUVEMENT_Tourner(float angle)
{
    this->typeMouvement = DEPLACEMENT_MOUVEMENT_ANGULAIRE;
    MOUVEMENT_ANGULAIRE_Tourner(angle);
}

void MOUVEMENT_RotationInfinie(float vitesse)
{
    this->typeMouvement = DEPLACEMENT_MOUVEMENT_ANGULAIRE;
    MOUVEMENT_ANGULAIRE_RotationInfinie(vitesse);
}

void MOUVEMENT_PointerVers(float x, float y)
{
    this->typeMouvement = DEPLACEMENT_MOUVEMENT_ANGULAIRE;
    MOUVEMENT_ANGULAIRE_PointerVers(x, y);
}

void MOUVEMENT_TangenterCercle(float x, float y, DEPLACEMENT_SENS_ROTATION sens)
{
    this->typeMouvement = DEPLACEMENT_MOUVEMENT_ANGULAIRE;
    MOUVEMENT_ANGULAIRE_TangenterCercle(x, y, sens);
}

void MOUVEMENT_AllerA(float x, float y, float vitesse)
{
    this->typeMouvement = DEPLACEMENT_MOUVEMENT_COMPLEXE;
    MOUVEMENT_COMPLEXE_AllerA(x, y, vitesse);
}

void MOUVEMENT_AllerADecompose(float x, float y, float vitesse)
{
    this->typeMouvement = DEPLACEMENT_MOUVEMENT_COMPLEXE;
    MOUVEMENT_COMPLEXE_AllerADecompose(x, y, vitesse);
}

void MOUVEMENT_PasserPar(float x, float y, float vitesse)
{
    this->typeMouvement = DEPLACEMENT_MOUVEMENT_COMPLEXE;
    MOUVEMENT_COMPLEXE_PasserPar(x, y, vitesse);
}

void MOUVEMENT_TournerAutourDe(float x, float y, DEPLACEMENT_SENS_ROTATION sens, float vitesseLongitudinale)
{
    this->typeMouvement = DEPLACEMENT_MOUVEMENT_COMPLEXE;
    MOUVEMENT_COMPLEXE_TournerAutourDe(x, y, sens, vitesseLongitudinale);
}

void MOUVEMENT_ChangerConsigne(DEPLACEMENT_CONSIGNE_TYPE type, DEPLACEMENT_CONSIGNE_PARAMETRES * parametres)
{
    switch(type)
    {
        case DEPLACEMENT_CONSIGNE_AVANCER:
            MOUVEMENT_Avancer(parametres->avancer.distance);
            break;
            
        case DEPLACEMENT_CONSIGNE_TRANSLATION_INFINIE:
            MOUVEMENT_TranslationInfinie(parametres->translationInfinie.vitesse);
            break;
            
        case DEPLACEMENT_CONSIGNE_POINTER_VERS:
            MOUVEMENT_PointerVers(parametres->pointerVers.x, parametres->pointerVers.y);
            break;
            
        case DEPLACEMENT_CONSIGNE_TANGENTER_CERCLE:
            MOUVEMENT_TangenterCercle(parametres->tangenterCercle.x, parametres->tangenterCercle.y, parametres->tangenterCercle.sens);
            break;
            
        case DEPLACEMENT_CONSIGNE_TOURNER:
            MOUVEMENT_Tourner(parametres->tourner.angle);
            break;
            
        case DEPLACEMENT_CONSIGNE_ROTATION_INFINIE:
            MOUVEMENT_RotationInfinie(parametres->rotationInfinie.vitesse);
            break;
            
        case DEPLACEMENT_CONSIGNE_ALLER_A:
            MOUVEMENT_AllerA(parametres->allerA.x, parametres->allerA.y, parametres->allerA.vitesse);
            break;
            
        case DEPLACEMENT_CONSIGNE_ALLER_A_DECOMPOSE:
            MOUVEMENT_AllerADecompose(parametres->AllerADecompose.x, parametres->AllerADecompose.y, parametres->AllerADecompose.vitesse);
            break;
            
        case DEPLACEMENT_CONSIGNE_PASSER_PAR:
            MOUVEMENT_PasserPar(parametres->passerPar.x, parametres->passerPar.y, parametres->passerPar.vitesse);
            break;
            
        case DEPLACEMENT_CONSIGNE_TOURNER_AUTOUR_DE:
            MOUVEMENT_TournerAutourDe(parametres->tournerAutourDe.x, parametres->tournerAutourDe.y, parametres->tournerAutourDe.sens,
                                      parametres->tournerAutourDe.vitesse);
            break;
            
        case DEPLACEMENT_CONSIGNE_STOP:
            MOUVEMENT_Stop();
            break;
            
        default:
            return;
            break;
    }
}

DEPLACEMENT_MOUVEMENT_ETAT MOUVEMENT_GetEtatMouvement()
{
    return this->etat.etatMouvement;
}

void MOUVEMENT_Run()
{
    DEPLACEMENT_MOUVEMENT_ETAT etatMouvementPrecedent = this->etat.etatMouvement;
    
    MOUVEMENT_ActualiserEtat();
    MOUVEMENT_CalculerEtAppliquerCommande();
    
    if(this->typeMouvement == DEPLACEMENT_MOUVEMENT_ANGULAIRE)
    {
        MOUVEMENT_ANGULAIRE_Run();
    }
    else if(this->typeMouvement == DEPLACEMENT_MOUVEMENT_LONGITUDINAL)
    {
        MOUVEMENT_LONGITUDINAL_Run();
    }
    else if (this->typeMouvement == DEPLACEMENT_MOUVEMENT_COMPLEXE)
    {
        MOUVEMENT_COMPLEXE_Run();
    }
    else if (this->typeMouvement == DEPLACEMENT_MOUVEMENT_ARRET)
    {
        ASSERVISSEMENT_Run(&this->membres.asservissementLongitudinal, ODOMETRIE_GetVitesseLongitudinale(), ODOMETRIE_GetPositionLongitudinale());
        ASSERVISSEMENT_Run(&this->membres.asservissementAngulaire   , ODOMETRIE_GetVitesseAngulaire()    , ODOMETRIE_GetPositionAngulaire());
    }
}


