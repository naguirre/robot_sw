#include <app/mouvement_longitudinal.h>
#include <stdio.h>

/*************************************/
/* SINGLETON                         */
/*************************************/

MOUVEMENT_LONGITUDINAL mouvementLongitudinal;
MOUVEMENT_LONGITUDINAL * this = &mouvementLongitudinal;

/*************************************/
/* STRUCTURE MACHINE A ETAT          */
/*************************************/

//PROTOTYPES DES FONCTIONS D'ETATS
void MAE_MOUVEMENT_LONGITUDINAL_EnCours();
void MAE_MOUVEMENT_LONGITUDINAL_PrecisionAtteinte();
void MAE_MOUVEMENT_LONGITUDINAL_Termine();
void MAE_MOUVEMENT_LONGITUDINAL_Blocage();
void MAE_MOUVEMENT_LONGITUDINAL_Dodo();

//GENERATION DE LA STRUCT MAE
#define MAE_NOM          MAE_MOUVEMENT_LONGITUDINAL
#define MAE_LISTE_ETATS  ETAT(MAE_MOUVEMENT_LONGITUDINAL_EnCours) \
                         ETAT(MAE_MOUVEMENT_LONGITUDINAL_PrecisionAtteinte) \
                         ETAT(MAE_MOUVEMENT_LONGITUDINAL_Termine) \
                         ETAT(MAE_MOUVEMENT_LONGITUDINAL_Dodo) \
                         DERNIER_ETAT(MAE_MOUVEMENT_LONGITUDINAL_Blocage)
#include <mae/createmae.h>


/*************************************/
/* FONCTIONS                         */
/*************************************/

void MOUVEMENT_LONGITUDINAL_Init(ASSERVISSEMENT * longitudinal, ASSERVISSEMENT * angulaire)
{
    this->membres.asservissementAngulaire = angulaire;
    this->membres.asservissementLongitudinal = longitudinal;
    MAE_Init(&this->membres.mae, ETAT_MAE_MOUVEMENT_LONGITUDINAL_Dodo, 0, 0, ETAT_MAE_MOUVEMENT_LONGITUDINAL_Dodo,
             MAE_MOUVEMENT_LONGITUDINAL_NB_ETATS, MAE_MOUVEMENT_LONGITUDINAL_Fonctions, this);
    
    
    this->etat.etatMouvement = DEPLACEMENT_MOUVEMENT_STOP;
    this->etat.nbBlocages = 0;
}


/******** CALCULS DE CONSIGNES ***********/

void MOUVEMENT_LONGITUDINAL_Avancer(float distance)
{
    float positionAngulaire = ODOMETRIE_GetPositionAngulaire();
    float consigneLongitudinale;
    
    consigneLongitudinale = ODOMETRIE_GetPositionLongitudinale() + distance;
    
    this->consigne.type = DEPLACEMENT_CONSIGNE_AVANCER;
    this->consigne.parametres.avancer.distance = distance;
    ASSERVISSEMENT_SetConsigne(this->membres.asservissementLongitudinal, ASSERVISSEMENT_POSITION, consigneLongitudinale);
    ASSERVISSEMENT_SetConsigne(this->membres.asservissementAngulaire, ASSERVISSEMENT_POSITION, positionAngulaire);
    
    MAE_ChangerEtat(&this->membres.mae, ETAT_MAE_MOUVEMENT_LONGITUDINAL_EnCours, 0, MAE_MOUVEMENT_LONGITUDINAL_TIMEOUT);
}

void MOUVEMENT_LONGITUDINAL_TranslationInfinie(float vitesse)
{
    float positionAngulaire = ODOMETRIE_GetPositionAngulaire();
    
    this->consigne.type = DEPLACEMENT_CONSIGNE_TRANSLATION_INFINIE;
    this->consigne.parametres.translationInfinie.vitesse = vitesse;
    
    ASSERVISSEMENT_SetConsigne(this->membres.asservissementLongitudinal, ASSERVISSEMENT_VITESSE, vitesse);
    ASSERVISSEMENT_SetConsigne(this->membres.asservissementAngulaire, ASSERVISSEMENT_POSITION, positionAngulaire);
    
    MAE_ChangerEtat(&this->membres.mae, ETAT_MAE_MOUVEMENT_LONGITUDINAL_EnCours, 0, MAE_MOUVEMENT_LONGITUDINAL_TIMEOUT);
}

void MOUVEMENT_LONGITUDINAL_AppliquerConsigneDeblocage()
{
    //On met les deux asserv en vitesse à 0 => on autorise le dérapage sans forcage
    ASSERVISSEMENT_SetConsigne(this->membres.asservissementLongitudinal, ASSERVISSEMENT_VITESSE, 0.0);
    ASSERVISSEMENT_SetConsigne(this->membres.asservissementAngulaire, ASSERVISSEMENT_VITESSE, 0.0);
}


/******** CONDITIONS DE TRANSITIONS ***********/

BOOL MOUVEMENT_LONGITUDINAL_VerifierBlocage()
{
    if(MATHS_EstDansBorne(ODOMETRIE_GetVitesseLongitudinale(), MOUVEMENT_LONGITUDINAL_SEUIL_BLOCAGE))
    {
        if(++this->etat.nbBlocages > MOUVEMENT_LONGITUDINAL_MAX_BLOCAGE)
        {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL MOUVEMENT_LONGITUDINAL_VerifierPrecisionAtteinte()
{
    float consigne = ASSERVISSEMENT_GetConsigneValeur(this->membres.asservissementLongitudinal);
    
    return MATHS_EstDansBorne(ODOMETRIE_GetPositionLongitudinale() - consigne, MOUVEMENT_LONGITUDINAL_SEUIL_PRECISION);
}

BOOL MOUVEMENT_LONGITUDINAL_VerifierTermine()
{
    return MATHS_EstDansBorne(ODOMETRIE_GetVitesseLongitudinale(), MOUVEMENT_LONGITUDINAL_SEUIL_TERMINE);
}


/******** MACHINE A ETATS ***********/

void MAE_MOUVEMENT_LONGITUDINAL_EnCours(void * p)
{
    //On signale le changement d'etat
    if(MAE_PremierPassage(&this->membres.mae))
    {
        fprintf(stderr, "DEPLACEMENT | Mouvement En Cours\n");
        this->etat.nbBlocages = 0;
        this->etat.etatMouvement = DEPLACEMENT_MOUVEMENT_EN_COURS;
    }
    
    if(MOUVEMENT_LONGITUDINAL_VerifierBlocage())
    {
        MAE_ChangerEtat(&this->membres.mae, ETAT_MAE_MOUVEMENT_LONGITUDINAL_Blocage, 0, MAE_MOUVEMENT_LONGITUDINAL_TIMEOUT);
    }
    
    //Mouvements de type position
    if(this->consigne.type == DEPLACEMENT_CONSIGNE_AVANCER)
    {
        if (MOUVEMENT_LONGITUDINAL_VerifierPrecisionAtteinte())
        {
            MAE_ChangerEtat(&this->membres.mae, ETAT_MAE_MOUVEMENT_LONGITUDINAL_PrecisionAtteinte, 0, MAE_MOUVEMENT_LONGITUDINAL_TIMEOUT);
        }
    }
}

void MAE_MOUVEMENT_LONGITUDINAL_PrecisionAtteinte(void * p)
{
    if(MAE_PremierPassage(&this->membres.mae))
    {
        fprintf(stderr, "DEPLACEMENT | Precision Atteinte\n");
        this->etat.etatMouvement = DEPLACEMENT_MOUVEMENT_PRECISION_ATTEINTE;
    }
    
    //mouvement de position
    if(this->consigne.type == DEPLACEMENT_CONSIGNE_AVANCER)
    {
        if (MOUVEMENT_LONGITUDINAL_VerifierTermine())
        {
            MAE_ChangerEtat(&this->membres.mae, ETAT_MAE_MOUVEMENT_LONGITUDINAL_Termine, 0, MAE_MOUVEMENT_LONGITUDINAL_TIMEOUT);
        }
    }
}

void MAE_MOUVEMENT_LONGITUDINAL_Termine(void * p)
{
    if(MAE_PremierPassage(&this->membres.mae))
    {
        fprintf(stderr, "DEPLACEMENT | Mouvement Termine\n");
        this->etat.etatMouvement = DEPLACEMENT_MOUVEMENT_TERMINE;
    }
}

void MAE_MOUVEMENT_LONGITUDINAL_Blocage(void * p)
{
    if(MAE_PremierPassage(&this->membres.mae))
    {
        fprintf(stderr, "DEPLACEMENT | Bloque\n");
        this->etat.etatMouvement = DEPLACEMENT_MOUVEMENT_BLOQUE;
        MOUVEMENT_LONGITUDINAL_AppliquerConsigneDeblocage();
    }
}

void MAE_MOUVEMENT_LONGITUDINAL_Dodo(void * p)
{
    return;
}

DEPLACEMENT_MOUVEMENT_ETAT MOUVEMENT_LONGITUDINAL_GetEtatDeplacement()
{
    return this->etat.etatMouvement;
}

void MOUVEMENT_LONGITUDINAL_Run()
{
    MAE_Automate(&this->membres.mae);
}
