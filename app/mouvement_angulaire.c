#include <app/mouvement_angulaire.h>
#include <stdio.h>

/*************************************/
/* SINGLETON                         */
/*************************************/

MOUVEMENT_ANGULAIRE mouvementAngulaire;
static MOUVEMENT_ANGULAIRE * this = &mouvementAngulaire;

/*************************************/
/* STRUCTURE MACHINE A ETAT          */
/*************************************/

//PROTOTYPES DES FONCTIONS D'ETATS
void MAE_MOUVEMENT_ANGULAIRE_EnCours();
void MAE_MOUVEMENT_ANGULAIRE_PrecisionAtteinte();
void MAE_MOUVEMENT_ANGULAIRE_Termine();
void MAE_MOUVEMENT_ANGULAIRE_Blocage();
void MAE_MOUVEMENT_ANGULAIRE_Dodo();

//GENERATION DE LA STRUCT MAE
#define MAE_NOM          MAE_MOUVEMENT_ANGULAIRE
#define MAE_LISTE_ETATS  ETAT(MAE_MOUVEMENT_ANGULAIRE_EnCours) \
                         ETAT(MAE_MOUVEMENT_ANGULAIRE_PrecisionAtteinte) \
                         ETAT(MAE_MOUVEMENT_ANGULAIRE_Termine) \
                         ETAT(MAE_MOUVEMENT_ANGULAIRE_Dodo) \
                         DERNIER_ETAT(MAE_MOUVEMENT_ANGULAIRE_Blocage)
#include <mae/createmae.h>


/*************************************/
/* FONCTIONS                         */
/*************************************/

void MOUVEMENT_ANGULAIRE_Init(ASSERVISSEMENT * longitudinal, ASSERVISSEMENT * angulaire)
{
    this->membres.asservissementAngulaire = angulaire;
    this->membres.asservissementLongitudinal = longitudinal;
    MAE_Init(&this->membres.mae, ETAT_MAE_MOUVEMENT_ANGULAIRE_Dodo, 0, 0, ETAT_MAE_MOUVEMENT_ANGULAIRE_Dodo,
             MAE_MOUVEMENT_ANGULAIRE_NB_ETATS, MAE_MOUVEMENT_ANGULAIRE_Fonctions, this);
    
    
    this->etat.etatMouvement = DEPLACEMENT_MOUVEMENT_STOP;
    this->etat.nbBlocages = 0;
}


/******** CALCULS DE CONSIGNES ***********/

void MOUVEMENT_ANGULAIRE_Tourner(float angle)
{
    float positionLongitudinale = ODOMETRIE_GetPositionLongitudinale();
    float consigneAngulaire;

    consigneAngulaire = ODOMETRIE_GetPositionAngulaire() + angle;
    
    this->consigne.type = DEPLACEMENT_CONSIGNE_TOURNER;
    this->consigne.parametres.tourner.angle = angle;
    ASSERVISSEMENT_SetConsigne(this->membres.asservissementLongitudinal, ASSERVISSEMENT_POSITION, positionLongitudinale);
    ASSERVISSEMENT_SetConsigne(this->membres.asservissementAngulaire, ASSERVISSEMENT_POSITION, consigneAngulaire);
    
    MAE_ChangerEtat(&this->membres.mae, ETAT_MAE_MOUVEMENT_ANGULAIRE_EnCours, 0, MAE_MOUVEMENT_ANGULAIRE_TIMEOUT);
}

void MOUVEMENT_ANGULAIRE_PointerVers(float x, float y)
{
    float positionLongitudinale = ODOMETRIE_GetPositionLongitudinale();
    float consigneAngulaire = ODOMETRIE_GetPositionAngulaire() + MATHS_DeltaCap(ODOMETRIE_GetTheta(), ODOMETRIE_GetX(), ODOMETRIE_GetY(), x, y);
    
    this->consigne.type = DEPLACEMENT_CONSIGNE_POINTER_VERS;
    this->consigne.parametres.pointerVers.x = x;
    this->consigne.parametres.pointerVers.y = y;
    
    ASSERVISSEMENT_SetConsigne(this->membres.asservissementLongitudinal, ASSERVISSEMENT_POSITION, positionLongitudinale);
    ASSERVISSEMENT_SetConsigne(this->membres.asservissementAngulaire, ASSERVISSEMENT_POSITION, consigneAngulaire);
    
    MAE_ChangerEtat(&this->membres.mae, ETAT_MAE_MOUVEMENT_ANGULAIRE_EnCours, 0, MAE_MOUVEMENT_ANGULAIRE_TIMEOUT);
}

void MOUVEMENT_ANGULAIRE_PointerDos(float x, float y)
{
    float positionLongitudinale = ODOMETRIE_GetPositionLongitudinale();
    float consigneAngulaire = ODOMETRIE_GetPositionAngulaire() + MATHS_DeltaCapContraire(ODOMETRIE_GetTheta(), ODOMETRIE_GetX(), ODOMETRIE_GetY(),
                                                                                         x, y);
    
    this->consigne.type = DEPLACEMENT_CONSIGNE_POINTER_VERS;
    this->consigne.parametres.pointerVers.x = x;
    this->consigne.parametres.pointerVers.y = y;
    
    ASSERVISSEMENT_SetConsigne(this->membres.asservissementLongitudinal, ASSERVISSEMENT_POSITION, positionLongitudinale);
    ASSERVISSEMENT_SetConsigne(this->membres.asservissementAngulaire, ASSERVISSEMENT_POSITION, consigneAngulaire);
    
    MAE_ChangerEtat(&this->membres.mae, ETAT_MAE_MOUVEMENT_ANGULAIRE_EnCours, 0, MAE_MOUVEMENT_ANGULAIRE_TIMEOUT);
}

void MOUVEMENT_ANGULAIRE_TangenterCercle(float x, float y, DEPLACEMENT_SENS_ROTATION sens)
{
    float positionLongitudinale = ODOMETRIE_GetPositionLongitudinale();
    float consigneAng;
    
    if(sens == DEPLACEMENT_SENS_TRIGO)
    {
        consigneAng = ODOMETRIE_GetPositionAngulaire() + MATHS_mod2pi(MATHS_DeltaCap(ODOMETRIE_GetTheta(), ODOMETRIE_GetX(), ODOMETRIE_GetY(), x, y) - M_PI_2);
    }else{
        consigneAng = ODOMETRIE_GetPositionAngulaire() + MATHS_mod2pi(MATHS_DeltaCap(ODOMETRIE_GetTheta(), ODOMETRIE_GetX(), ODOMETRIE_GetY(), x, y) + M_PI_2);
    }
    
    this->consigne.type = DEPLACEMENT_CONSIGNE_TANGENTER_CERCLE;
    this->consigne.parametres.tangenterCercle.x = x;
    this->consigne.parametres.tangenterCercle.y = y;
    this->consigne.parametres.tangenterCercle.sens = sens;
    
    ASSERVISSEMENT_SetConsigne(this->membres.asservissementLongitudinal, ASSERVISSEMENT_POSITION, positionLongitudinale);
    ASSERVISSEMENT_SetConsigne(this->membres.asservissementAngulaire, ASSERVISSEMENT_POSITION, consigneAng);
    
    MAE_ChangerEtat(&this->membres.mae, ETAT_MAE_MOUVEMENT_ANGULAIRE_EnCours, 0, MAE_MOUVEMENT_ANGULAIRE_TIMEOUT);
}

void MOUVEMENT_ANGULAIRE_RotationInfinie(float vitesse)
{
    float positionLongitudinale = ODOMETRIE_GetPositionLongitudinale();
    
    this->consigne.type = DEPLACEMENT_CONSIGNE_ROTATION_INFINIE;
    this->consigne.parametres.rotationInfinie.vitesse = vitesse;
    
    ASSERVISSEMENT_SetConsigne(this->membres.asservissementLongitudinal, ASSERVISSEMENT_POSITION, positionLongitudinale);
    ASSERVISSEMENT_SetConsigne(this->membres.asservissementAngulaire, ASSERVISSEMENT_VITESSE, vitesse);
    
    MAE_ChangerEtat(&this->membres.mae, ETAT_MAE_MOUVEMENT_ANGULAIRE_EnCours, 0, MAE_MOUVEMENT_ANGULAIRE_TIMEOUT);
}

void MOUVEMENT_ANGULAIRE_AppliquerConsigneDeblocage()
{
    //On met les deux asserv en vitesse à 0 => on autorise le dérapage sans forcage
    ASSERVISSEMENT_SetConsigne(this->membres.asservissementLongitudinal, ASSERVISSEMENT_VITESSE, 0.0);
    ASSERVISSEMENT_SetConsigne(this->membres.asservissementAngulaire, ASSERVISSEMENT_VITESSE, 0.0);
}


/******** CONDITIONS DE TRANSITIONS ***********/

BOOL MOUVEMENT_ANGULAIRE_VerifierBlocage()
{
    if(MATHS_EstDansBorne(ODOMETRIE_GetVitesseAngulaire(), MOUVEMENT_ANGULAIRE_SEUIL_BLOCAGE))
    {
        if(++this->etat.nbBlocages > MOUVEMENT_ANGULAIRE_MAX_BLOCAGE)
        {
            return TRUE;
        }
    }
    
    return FALSE;
}

BOOL MOUVEMENT_ANGULAIRE_VerifierPrecisionAtteinte()
{
    float consigne = ASSERVISSEMENT_GetConsigneValeur(this->membres.asservissementAngulaire);
    
    return MATHS_EstDansBorne(ODOMETRIE_GetPositionAngulaire() - consigne, MOUVEMENT_ANGULAIRE_SEUIL_PRECISION);
}

BOOL MOUVEMENT_ANGULAIRE_VerifierTermine()
{
    return MATHS_EstDansBorne(ODOMETRIE_GetVitesseAngulaire(), MOUVEMENT_ANGULAIRE_SEUIL_TERMINE);
}

/******** MACHINE A ETATS ***********/


void MAE_MOUVEMENT_ANGULAIRE_EnCours(void * p)
{
    //On signale le changement d'etat
    if(MAE_PremierPassage(&this->membres.mae))
    {
        fprintf(stderr, "DEPLACEMENT | ANGULAIRE | Mouvement En Cours\n");
        this->etat.nbBlocages = 0;
        this->etat.etatMouvement = DEPLACEMENT_MOUVEMENT_EN_COURS;
    }

    if(MOUVEMENT_ANGULAIRE_VerifierBlocage())
    {
        MAE_ChangerEtat(&this->membres.mae, ETAT_MAE_MOUVEMENT_ANGULAIRE_Blocage, 0, MAE_MOUVEMENT_ANGULAIRE_TIMEOUT);
    }
    
    //Mouvements de type position
    if((this->consigne.type == DEPLACEMENT_CONSIGNE_TOURNER) || (this->consigne.type == DEPLACEMENT_CONSIGNE_POINTER_VERS) ||
       (this->consigne.type == DEPLACEMENT_CONSIGNE_TANGENTER_CERCLE))
    {
        if (MOUVEMENT_ANGULAIRE_VerifierPrecisionAtteinte())
        {
            MAE_ChangerEtat(&this->membres.mae, ETAT_MAE_MOUVEMENT_ANGULAIRE_PrecisionAtteinte, 0, MAE_MOUVEMENT_ANGULAIRE_TIMEOUT);
        }
    }
    
}

void MAE_MOUVEMENT_ANGULAIRE_PrecisionAtteinte(void * p)
{
    if(MAE_PremierPassage(&this->membres.mae))
    {
        fprintf(stderr, "DEPLACEMENT | ANGULAIRE | Precision Atteinte\n");
        this->etat.etatMouvement = DEPLACEMENT_MOUVEMENT_PRECISION_ATTEINTE;
    }
    
    //mouvements de type position angulaire
    if((this->consigne.type == DEPLACEMENT_CONSIGNE_TOURNER) || (this->consigne.type == DEPLACEMENT_CONSIGNE_POINTER_VERS) ||
       (this->consigne.type == DEPLACEMENT_CONSIGNE_TANGENTER_CERCLE))
    {
        if (MOUVEMENT_ANGULAIRE_VerifierTermine())
        {
            MAE_ChangerEtat(&this->membres.mae, ETAT_MAE_MOUVEMENT_ANGULAIRE_Termine, 0, MAE_MOUVEMENT_ANGULAIRE_TIMEOUT);
        }
    }
}

void MAE_MOUVEMENT_ANGULAIRE_Termine(void * p)
{
    if(MAE_PremierPassage(&this->membres.mae))
    {
        fprintf(stderr, "DEPLACEMENT | ANGULAIRE | Mouvement Termine\n");
        this->etat.etatMouvement = DEPLACEMENT_MOUVEMENT_TERMINE;
    }
}

void MAE_MOUVEMENT_ANGULAIRE_Blocage(void * p)
{
    if(MAE_PremierPassage(&this->membres.mae))
    {
        fprintf(stderr, "DEPLACEMENT | ANGULAIRE | Bloque\n");
        this->etat.etatMouvement = DEPLACEMENT_MOUVEMENT_BLOQUE;
        MOUVEMENT_ANGULAIRE_AppliquerConsigneDeblocage();
    }
}

void MAE_MOUVEMENT_ANGULAIRE_Dodo(void * p)
{
    return;
}

/******* ACCESSEURS ********/

DEPLACEMENT_MOUVEMENT_ETAT MOUVEMENT_ANGULAIRE_GetEtatDeplacement()
{
    return this->etat.etatMouvement;
}

/******* RUN *********/

void MOUVEMENT_ANGULAIRE_Run()
{
    MAE_Automate(&this->membres.mae);
}


