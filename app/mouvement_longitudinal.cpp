#include <app/mouvement_longitudinal.h>
#include <stdio.h>

/*************************************/
/* SINGLETON                         */
/*************************************/

//MOUVEMENT_LONGITUDINAL mouvementLongitudinal;
//MOUVEMENT_LONGITUDINAL * this = &mouvementLongitudinal;

/*************************************/
/* STRUCTURE MACHINE A ETAT          */
/*************************************/

//PROTOTYPES DES FONCTIONS D'ETATS
void MAE_MOUVEMENT_LONGITUDINAL_EnCours();
void MAE_MOUVEMENT_LONGITUDINAL_PrecisionAtteinte();
void MAE_MOUVEMENT_LONGITUDINAL_Termine();
void MAE_MOUVEMENT_LONGITUDINAL_Blocage();
void MAE_MOUVEMENT_LONGITUDINAL_Dodo();

// //GENERATION DE LA STRUCT MAE
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

MOUVEMENT_LONGITUDINAL::MOUVEMENT_LONGITUDINAL(ASSERVISSEMENT * longitudinal, ASSERVISSEMENT * angulaire)
{
    membres.asservissementAngulaire = angulaire;
    membres.asservissementLongitudinal = longitudinal;
    membres.mae = new MAE(ETAT_MAE_MOUVEMENT_LONGITUDINAL_Dodo, 0, 0, ETAT_MAE_MOUVEMENT_LONGITUDINAL_Dodo,
             MAE_MOUVEMENT_LONGITUDINAL_NB_ETATS, MAE_MOUVEMENT_LONGITUDINAL_Fonctions, this);
    
    
    etat.etatMouvement = DEPLACEMENT_MOUVEMENT_STOP;
    etat.nbBlocages = 0;
}


/******** CALCULS DE CONSIGNES ***********/

void MOUVEMENT_LONGITUDINAL::Avancer(float distance)
{
    float positionAngulaire = ODOMETRIE::GetPositionAngulaire();
    float consigneLongitudinale;
    
    consigneLongitudinale = ODOMETRIE::GetPositionLongitudinale() + distance;
    
    consigne.type = DEPLACEMENT_CONSIGNE_AVANCER;
    consigne.parametres.avancer.distance = distance;
    membres.asservissementLongitudinal.SetConsigne(ASSERVISSEMENT_POSITION, consigneLongitudinale);
    membres.asservissementAngulaire.SetConsigne(ASSERVISSEMENT_POSITION, positionAngulaire);
    
    membres.mae->ChangerEtat(ETAT_MAE_MOUVEMENT_LONGITUDINAL_EnCours, 0, MAE_MOUVEMENT_LONGITUDINAL_TIMEOUT);
}

void MOUVEMENT_LONGITUDINAL::TranslationInfinie(float vitesse)
{
     float positionAngulaire = ODOMETRIE::GetPositionAngulaire();
    
    consigne.type = DEPLACEMENT_CONSIGNE_TRANSLATION_INFINIE;
    consigne.parametres.translationInfinie.vitesse = vitesse;
    
    membres.asservissementLongitudinal->SetConsigne(ASSERVISSEMENT_VITESSE, vitesse);
    membres.asservissementAngulaire->SetConsigne(ASSERVISSEMENT_POSITION, positionAngulaire);
    
    membres.mae->ChangerEtat(ETAT_MAE_MOUVEMENT_LONGITUDINAL_EnCours, 0, MAE_MOUVEMENT_LONGITUDINAL_TIMEOUT);
}

void MOUVEMENT_LONGITUDINAL::AppliquerConsigneDeblocage()
{
    //On met les deux asserv en vitesse à 0 => on autorise le dérapage sans forcage
    membres.asservissementLongitudinal.SetConsigne(ASSERVISSEMENT_VITESSE, 0.0);
    membres.asservissementAngulaire.SetConsigne(ASSERVISSEMENT_VITESSE, 0.0);
}


/******** CONDITIONS DE TRANSITIONS ***********/

BOOL MOUVEMENT_LONGITUDINAL::VerifierBlocage()
{
    if(MATHS_EstDansBorne(ODOMETRIE::GetVitesseLongitudinale(), MOUVEMENT_LONGITUDINAL_SEUIL_BLOCAGE))
    {
        if(++etat.nbBlocages > MOUVEMENT_LONGITUDINAL_MAX_BLOCAGE)
        {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL MOUVEMENT_LONGITUDINAL::VerifierPrecisionAtteinte()
{
    float consigne = ASSERVISSEMENT_GetConsigneValeur(membres.asservissementLongitudinal);
    
    return MATHS_EstDansBorne(ODOMETRIE::GetPositionLongitudinale() - consigne, MOUVEMENT_LONGITUDINAL_SEUIL_PRECISION);
}

BOOL MOUVEMENT_LONGITUDINAL::VerifierTermine()
{
  return MATHS_EstDansBorne(ODOMETRIE::GetVitesseLongitudinale(), MOUVEMENT_LONGITUDINAL_SEUIL_TERMINE);
}


/******** MACHINE A ETATS ***********/

void MAE_MOUVEMENT_LONGITUDINAL::EnCours(void * p)
{
    //On signale le changement d'etat
    if(membres.mae->PremierPassage())
    {
        fprintf(stderr, "DEPLACEMENT | Mouvement En Cours\n");
        etat.nbBlocages = 0;
        etat.etatMouvement = DEPLACEMENT_MOUVEMENT_EN_COURS;
    }
    
    if(MOUVEMENT_LONGITUDINAL_VerifierBlocage())
    {
        membres.mae->ChangerEtat(ETAT_MAE_MOUVEMENT_LONGITUDINAL_Blocage, 0, MAE_MOUVEMENT_LONGITUDINAL_TIMEOUT);
    }
    
    //Mouvements de type position
    if(consigne.type == DEPLACEMENT_CONSIGNE_AVANCER)
    {
        if (VerifierPrecisionAtteinte())
        {
            membres.mae->ChangerEtat(ETAT_MAE_MOUVEMENT_LONGITUDINAL_PrecisionAtteinte, 0, MAE_MOUVEMENT_LONGITUDINAL_TIMEOUT);
        }
    }
}

void MAE_MOUVEMENT_LONGITUDINAL::PrecisionAtteinte(void * p)
{
    if(membres.mae->PremierPassage())
    {
        fprintf(stderr, "DEPLACEMENT | Precision Atteinte\n");
        etat.etatMouvement = DEPLACEMENT_MOUVEMENT_PRECISION_ATTEINTE;
    }
    
    //mouvement de position
    if(consigne.type == DEPLACEMENT_CONSIGNE_AVANCER)
    {
        if (VerifierTermine())
        {
            membres.mae->ChangerEtat(ETAT_MAE_MOUVEMENT_LONGITUDINAL_Termine, 0, MAE_MOUVEMENT_LONGITUDINAL_TIMEOUT);
        }
    }
}

void MAE_MOUVEMENT_LONGITUDINAL::Termine(void * p)
{
    if(membres->mae->PremierPassage())
    {
        fprintf(stderr, "DEPLACEMENT | Mouvement Termine\n");
        etat.etatMouvement = DEPLACEMENT_MOUVEMENT_TERMINE;
    }
}

void MAE_MOUVEMENT_LONGITUDINAL::Blocage(void * p)
{
    if(membres.mae->PremierPassage())
    {
        fprintf(stderr, "DEPLACEMENT | Bloque\n");
        etat.etatMouvement = DEPLACEMENT_MOUVEMENT_BLOQUE;
	AppliquerConsigneDeblocage();
    }
}

void MAE_MOUVEMENT_LONGITUDINAL::Dodo(void * p)
{
    return;
}

DEPLACEMENT_MOUVEMENT_ETAT MOUVEMENT_LONGITUDINAL::GetEtatDeplacement()
{
    return etat.etatMouvement;
}

void MOUVEMENT_LONGITUDINAL_Run()
{
    membres.mae->Automate();
}
