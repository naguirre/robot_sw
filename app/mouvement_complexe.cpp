#include <app/mouvement_complexe.h>
#include <stdio.h>

/*************************************/
/* SINGLETON                         */
/*************************************/

MOUVEMENT_COMPLEXE mouvementComplexe;
static MOUVEMENT_COMPLEXE * this = &mouvementComplexe;


/*************************************/
/* STRUCTURE MACHINE A ETAT          */
/*************************************/

//PROTOTYPES DES FONCTIONS D'ETATS
void MAE_MOUVEMENT_COMPLEXE_RotationInitiale(void * p);
void MAE_MOUVEMENT_COMPLEXE_SuiviTrajectoireEnCours(void * p);
void MAE_MOUVEMENT_COMPLEXE_SuiviTrajectoirePrecisionAtteinte(void * p);
void MAE_MOUVEMENT_COMPLEXE_Blocage(void * p);
void MAE_MOUVEMENT_COMPLEXE_Stop(void * p);
void MAE_MOUVEMENT_COMPLEXE_Dodo(void * p);
void MAE_MOUVEMENT_COMPLEXE_Termine(void * p);

//GENERATION DE LA STRUCT MAE
#define MAE_NOM          MAE_MOUVEMENT_COMPLEXE
#define MAE_LISTE_ETATS  ETAT(MAE_MOUVEMENT_COMPLEXE_RotationInitiale) \
                         ETAT(MAE_MOUVEMENT_COMPLEXE_SuiviTrajectoireEnCours) \
                         ETAT(MAE_MOUVEMENT_COMPLEXE_SuiviTrajectoirePrecisionAtteinte) \
                         ETAT(MAE_MOUVEMENT_COMPLEXE_Blocage) \
                         ETAT(MAE_MOUVEMENT_COMPLEXE_Dodo) \
                         ETAT(MAE_MOUVEMENT_COMPLEXE_Termine) \
                         DERNIER_ETAT(MAE_MOUVEMENT_COMPLEXE_Stop)
#include <mae/createmae.h>


void MOUVEMENT_COMPLEXE_Init(ASSERVISSEMENT * longitudinal, ASSERVISSEMENT * angulaire)
{
    this->membres.asservissementAngulaire = angulaire;
    this->membres.asservissementLongitudinal = longitudinal;
    MAE_Init(&this->membres.mae, ETAT_MAE_MOUVEMENT_COMPLEXE_Dodo, 0, 0, ETAT_MAE_MOUVEMENT_COMPLEXE_Dodo,
             MAE_MOUVEMENT_COMPLEXE_NB_ETATS, MAE_MOUVEMENT_COMPLEXE_Fonctions, this);
    
    
    this->etat.etatMouvement = DEPLACEMENT_MOUVEMENT_STOP;
    this->etat.nbBlocages = 0;
}

void MOUVEMENT_COMPLEXE_ActualiserConsigne()
{
    float consigneLongitudinale, consigneAngulaire, angle, deltaAngle, xVise, yVise;
    
    switch (this->consigne.type)
    {
        case DEPLACEMENT_CONSIGNE_ALLER_A:
        case DEPLACEMENT_CONSIGNE_ALLER_A_DECOMPOSE:
            if(this->consigne.parametres.allerA.vitesse > 0.0)
            {
                consigneLongitudinale = ODOMETRIE_GetPositionLongitudinale() + MATHS_Distance(ODOMETRIE_GetX(), ODOMETRIE_GetY(), this->consigne.parametres.allerA.x, this->consigne.parametres.allerA.y);
                consigneAngulaire = ODOMETRIE_GetPositionAngulaire() + MATHS_DeltaCap(ODOMETRIE_GetTheta(), ODOMETRIE_GetX(), ODOMETRIE_GetY(), this->consigne.parametres.allerA.x, this->consigne.parametres.allerA.y);

            }else{
                consigneLongitudinale = ODOMETRIE_GetPositionLongitudinale() - MATHS_Distance(ODOMETRIE_GetX(), ODOMETRIE_GetY(), this->consigne.parametres.allerA.x, this->consigne.parametres.allerA.y);
                consigneAngulaire = ODOMETRIE_GetPositionAngulaire() + MATHS_DeltaCapContraire(ODOMETRIE_GetTheta(), ODOMETRIE_GetX(), ODOMETRIE_GetY(), this->consigne.parametres.allerA.x, this->consigne.parametres.allerA.y);
            }
            ASSERVISSEMENT_SetVitesseMax(this->membres.asservissementLongitudinal, this->consigne.parametres.allerA.vitesse);
            ASSERVISSEMENT_SetConsigne(this->membres.asservissementLongitudinal, ASSERVISSEMENT_POSITION, consigneLongitudinale);
            ASSERVISSEMENT_SetConsigne(this->membres.asservissementAngulaire, ASSERVISSEMENT_POSITION, consigneAngulaire);
            break;
            
        case DEPLACEMENT_CONSIGNE_PASSER_PAR:
            consigneLongitudinale = this->consigne.parametres.passerPar.vitesse;
            consigneAngulaire = ODOMETRIE_GetPositionAngulaire() + MATHS_DeltaCap(ODOMETRIE_GetTheta(), ODOMETRIE_GetX(), ODOMETRIE_GetY(), this->consigne.parametres.allerA.x, this->consigne.parametres.allerA.y);
            ASSERVISSEMENT_SetVitesseMax(this->membres.asservissementLongitudinal, this->consigne.parametres.AllerADecompose.vitesse);
            ASSERVISSEMENT_SetConsigne(this->membres.asservissementLongitudinal, ASSERVISSEMENT_VITESSE, consigneLongitudinale);
            ASSERVISSEMENT_SetConsigne(this->membres.asservissementAngulaire, ASSERVISSEMENT_POSITION, consigneAngulaire);
            break;
            
        case DEPLACEMENT_CONSIGNE_TOURNER_AUTOUR_DE:
            consigneLongitudinale = this->consigne.parametres.tournerAutourDe.vitesse;
            deltaAngle = (ODOMETRIE_GetVitesseLongitudinale())/(2.0*M_PI*this->consigne.parametres.tournerAutourDe.rayon)*0.05;
            angle = atan2f(ODOMETRIE_GetY() - this->consigne.parametres.tournerAutourDe.y, ODOMETRIE_GetX() - this->consigne.parametres.tournerAutourDe.x);
            if(this->consigne.parametres.tournerAutourDe.sens == DEPLACEMENT_SENS_TRIGO)
            {
                xVise = this->consigne.parametres.tournerAutourDe.x + this->consigne.parametres.tournerAutourDe.rayon*cosf(angle + deltaAngle);
                yVise = this->consigne.parametres.tournerAutourDe.y + this->consigne.parametres.tournerAutourDe.rayon*sinf(angle + deltaAngle);
            }
            else{
                xVise = this->consigne.parametres.tournerAutourDe.x + this->consigne.parametres.tournerAutourDe.rayon*cosf(angle - deltaAngle);
                yVise = this->consigne.parametres.tournerAutourDe.y + this->consigne.parametres.tournerAutourDe.rayon*sinf(angle - deltaAngle);
            }

            consigneAngulaire = ODOMETRIE_GetPositionAngulaire() + MATHS_DeltaCap(ODOMETRIE_GetTheta(), ODOMETRIE_GetX(), ODOMETRIE_GetY(),
                                                                                  xVise, yVise);
            
            ASSERVISSEMENT_SetConsigne(this->membres.asservissementLongitudinal, ASSERVISSEMENT_VITESSE, consigneLongitudinale);
            ASSERVISSEMENT_SetConsigne(this->membres.asservissementAngulaire, ASSERVISSEMENT_POSITION, consigneAngulaire);
            break;
            
        default:
            break;
    }
}

void MOUVEMENT_COMPLEXE_CalculerRotationInitiale()
{
    switch (this->consigne.type)
    {
        case DEPLACEMENT_CONSIGNE_ALLER_A:
            if(this->consigne.parametres.allerA.vitesse > 0.0)
                MOUVEMENT_ANGULAIRE_PointerVers(this->consigne.parametres.allerA.x, this->consigne.parametres.allerA.y);
            else
                MOUVEMENT_ANGULAIRE_PointerDos(this->consigne.parametres.allerA.x, this->consigne.parametres.allerA.y);
            break;
            
        case DEPLACEMENT_CONSIGNE_ALLER_A_DECOMPOSE:
            MOUVEMENT_ANGULAIRE_PointerVers(this->consigne.parametres.AllerADecompose.x, this->consigne.parametres.AllerADecompose.y);
            break;
        
        case DEPLACEMENT_CONSIGNE_PASSER_PAR:
            MOUVEMENT_ANGULAIRE_PointerVers(this->consigne.parametres.passerPar.x, this->consigne.parametres.passerPar.y);
            break;
            
        case DEPLACEMENT_CONSIGNE_TOURNER_AUTOUR_DE:
            MOUVEMENT_ANGULAIRE_TangenterCercle(this->consigne.parametres.tournerAutourDe.x, this->consigne.parametres.tournerAutourDe.y,
                                                this->consigne.parametres.tournerAutourDe.sens);
            break;
            
        default:
            break;
    }
}

BOOL MOUVEMENT_COMPLEXE_CibleEstDevant(float x, float y)
{
    float xActuel = ODOMETRIE_GetX();
    float yActuel = ODOMETRIE_GetY();
    float thetaActuel = ODOMETRIE_GetTheta();
    
    return (BOOL)(MATHS_abs(MATHS_DeltaCap(thetaActuel, xActuel, yActuel, x, y)) < M_PI_2);
}

void MOUVEMENT_COMPLEXE_AllerA(float x, float y, float vitesse)
{
    this->consigne.type = DEPLACEMENT_CONSIGNE_ALLER_A;
    this->consigne.parametres.allerA.x = x;
    this->consigne.parametres.allerA.y = y;
    this->consigne.parametres.allerA.vitesse = vitesse;
    this->etat.etatMouvement = DEPLACEMENT_MOUVEMENT_EN_COURS;
    
    if (!MOUVEMENT_COMPLEXE_CibleEstDevant(x, y))
    {
        MAE_ChangerEtat(&this->membres.mae, ETAT_MAE_MOUVEMENT_COMPLEXE_RotationInitiale, 0, MAE_MOUVEMENT_COMPLEXE_TIMEOUT);
    }else{
        MAE_ChangerEtat(&this->membres.mae, ETAT_MAE_MOUVEMENT_COMPLEXE_SuiviTrajectoireEnCours, 0, MAE_MOUVEMENT_COMPLEXE_TIMEOUT);
    }
}

void MOUVEMENT_COMPLEXE_AllerADecompose(float x, float y, float vitesse)
{
    this->consigne.type = DEPLACEMENT_CONSIGNE_ALLER_A_DECOMPOSE;
    this->consigne.parametres.AllerADecompose.x = x;
    this->consigne.parametres.AllerADecompose.y = y;
    this->consigne.parametres.AllerADecompose.vitesse = vitesse;
    this->etat.etatMouvement = DEPLACEMENT_MOUVEMENT_EN_COURS;
    
    MOUVEMENT_COMPLEXE_CalculerRotationInitiale();
    MAE_ChangerEtat(&this->membres.mae, ETAT_MAE_MOUVEMENT_COMPLEXE_RotationInitiale, 0, MAE_MOUVEMENT_COMPLEXE_TIMEOUT);
    
}

void MOUVEMENT_COMPLEXE_PasserPar(float x, float y, float vitesse)
{
    this->consigne.type = DEPLACEMENT_CONSIGNE_PASSER_PAR;
    this->consigne.parametres.passerPar.x = x;
    this->consigne.parametres.passerPar.y = y;
    this->consigne.parametres.passerPar.vitesse = vitesse;
    this->etat.etatMouvement = DEPLACEMENT_MOUVEMENT_EN_COURS;
    
    if (!MOUVEMENT_COMPLEXE_CibleEstDevant(x, y))
    {
        MAE_ChangerEtat(&this->membres.mae, ETAT_MAE_MOUVEMENT_COMPLEXE_RotationInitiale, 0, MAE_MOUVEMENT_COMPLEXE_TIMEOUT);
    }else{
        MAE_ChangerEtat(&this->membres.mae, ETAT_MAE_MOUVEMENT_COMPLEXE_SuiviTrajectoireEnCours, 0, MAE_MOUVEMENT_COMPLEXE_TIMEOUT);
    }
}

void MOUVEMENT_COMPLEXE_TournerAutourDe(float x, float y, DEPLACEMENT_SENS_ROTATION sens, float vitesseLongitudinale)
{
    this->consigne.type = DEPLACEMENT_CONSIGNE_TOURNER_AUTOUR_DE;
    this->consigne.parametres.tournerAutourDe.x = x;
    this->consigne.parametres.tournerAutourDe.y = y;
    this->consigne.parametres.tournerAutourDe.sens = sens;
    this->consigne.parametres.tournerAutourDe.vitesse = vitesseLongitudinale;
    this->consigne.parametres.tournerAutourDe.rayon = MATHS_Distance(ODOMETRIE_GetX(), ODOMETRIE_GetY(), x, y);
    this->etat.etatMouvement = DEPLACEMENT_MOUVEMENT_EN_COURS;
    
    MOUVEMENT_COMPLEXE_CalculerRotationInitiale();
    MAE_ChangerEtat(&this->membres.mae, ETAT_MAE_MOUVEMENT_COMPLEXE_RotationInitiale, 0, MAE_MOUVEMENT_COMPLEXE_TIMEOUT);
}

BOOL MOUVEMENT_COMPLEXE_VerifierBlocage()
{
    if (MATHS_EstDansBorne(ODOMETRIE_GetVitesseLongitudinale(), MOUVEMENT_LONGITUDINAL_SEUIL_BLOCAGE))
    {
        if (++this->etat.nbBlocages > MOUVEMENT_LONGITUDINAL_MAX_BLOCAGE)
        {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL MOUVEMENT_COMPLEXE_VerifierPrecisionAtteinte()
{
    switch (this->consigne.type)
    {
        case DEPLACEMENT_CONSIGNE_PASSER_PAR:
            if (MATHS_Distance(ODOMETRIE_GetX(), ODOMETRIE_GetY(), this->consigne.parametres.passerPar.x, this->consigne.parametres.passerPar.y) < 0.1)
            {
                return TRUE;
            }else{
                return FALSE;
            }
            break;
            
        case DEPLACEMENT_CONSIGNE_ALLER_A:
            if (MATHS_Distance(ODOMETRIE_GetX(), ODOMETRIE_GetY(), this->consigne.parametres.allerA.x, this->consigne.parametres.allerA.y) < 0.05)
            {
                return TRUE;
            }else{
                return FALSE;
            }
            break;
            
        case DEPLACEMENT_CONSIGNE_ALLER_A_DECOMPOSE:
            if (MATHS_Distance(ODOMETRIE_GetX(), ODOMETRIE_GetY(), this->consigne.parametres.AllerADecompose.x, this->consigne.parametres.AllerADecompose.y) < 0.01)
            {
                return TRUE;
            }else{
                return FALSE;
            }
            break;
            
        default:
            return FALSE;
            break;
    }
}

BOOL MOUVEMENT_COMPLEXE_VerifierTermine()
{
    if ((MATHS_EstDansBorne(ODOMETRIE_GetVitesseAngulaire(), MOUVEMENT_ANGULAIRE_SEUIL_TERMINE)) &&
        (MATHS_EstDansBorne(ODOMETRIE_GetVitesseLongitudinale(), MOUVEMENT_LONGITUDINAL_SEUIL_TERMINE)) &&
        (MAE_GetEtatActuel(&this->membres.mae) == ETAT_MAE_MOUVEMENT_COMPLEXE_SuiviTrajectoirePrecisionAtteinte))
    {
        return TRUE;
    }else{
        return FALSE;
    }
}


void MAE_MOUVEMENT_COMPLEXE_RotationInitiale(void * p)
{
    if (MAE_PremierPassage(&this->membres.mae))
    {
        MOUVEMENT_COMPLEXE_CalculerRotationInitiale();
        fprintf(stderr, "DEPLACEMENT | MOUVEMENT COMPLEXE | Rotation initiale\n");
    }
    
    MOUVEMENT_ANGULAIRE_Run();
    
    if ((MOUVEMENT_ANGULAIRE_GetEtatDeplacement() == DEPLACEMENT_MOUVEMENT_TERMINE) ||
        (MOUVEMENT_ANGULAIRE_GetEtatDeplacement() == DEPLACEMENT_MOUVEMENT_PRECISION_ATTEINTE))
    {
        MAE_ChangerEtat(&this->membres.mae, ETAT_MAE_MOUVEMENT_COMPLEXE_SuiviTrajectoireEnCours, 0, MAE_MOUVEMENT_COMPLEXE_TIMEOUT);
    }
    else if (MOUVEMENT_ANGULAIRE_GetEtatDeplacement() == DEPLACEMENT_MOUVEMENT_BLOQUE)
    {
        MAE_ChangerEtat(&this->membres.mae, ETAT_MAE_MOUVEMENT_COMPLEXE_Blocage, 0, 0);
    }
}

void MAE_MOUVEMENT_COMPLEXE_SuiviTrajectoireEnCours(void * p)
{
    if (MAE_PremierPassage(&this->membres.mae))
    {
        fprintf(stderr, "DEPLACEMENT | MOUVEMENT COMPLEXE | Suivi Trajetoire en cours\n");
        this->etat.nbBlocages = 0;
        this->etat.etatMouvement = DEPLACEMENT_MOUVEMENT_EN_COURS;
    }
    
    MOUVEMENT_COMPLEXE_ActualiserConsigne();
    
    if(MOUVEMENT_COMPLEXE_VerifierPrecisionAtteinte())
    {
        MAE_ChangerEtat(&this->membres.mae, ETAT_MAE_MOUVEMENT_COMPLEXE_SuiviTrajectoirePrecisionAtteinte, 0, 0);
    }
    else if(MOUVEMENT_COMPLEXE_VerifierBlocage())
    {
        MAE_ChangerEtat(&this->membres.mae, ETAT_MAE_MOUVEMENT_COMPLEXE_Blocage, 0, 0);
    }
    
}

void MAE_MOUVEMENT_COMPLEXE_SuiviTrajectoirePrecisionAtteinte(void * p)
{
    if (MAE_PremierPassage(&this->membres.mae))
    {
        fprintf(stderr, "DEPLACEMENT | MOUVEMENT COMPLEXE | Suivi Trajetoire Precision Atteinte\n");
        this->etat.etatMouvement = DEPLACEMENT_MOUVEMENT_PRECISION_ATTEINTE;
    }
    MOUVEMENT_COMPLEXE_ActualiserConsigne();
    //On arrete de corriger l'angle
    //ASSERVISSEMENT_SetConsigne(this->membres.asservissementAngulaire, ASSERVISSEMENT_POSITION, ODOMETRIE_GetPositionAngulaire());
    
    if(MOUVEMENT_COMPLEXE_VerifierTermine())
    {
        MAE_ChangerEtat(&this->membres.mae, ETAT_MAE_MOUVEMENT_COMPLEXE_Termine, 0, 0);
    }
    
}

void MAE_MOUVEMENT_COMPLEXE_Blocage(void * p)
{
    if (MAE_PremierPassage(&this->membres.mae))
    {
        this->etat.etatMouvement = DEPLACEMENT_MOUVEMENT_BLOQUE;
        fprintf(stderr, "DEPLACEMENT | MOUVEMENT COMPLEXE | Blocage\n");
        ASSERVISSEMENT_SetConsigne(this->membres.asservissementLongitudinal, ASSERVISSEMENT_VITESSE, 0.0);
        ASSERVISSEMENT_SetConsigne(this->membres.asservissementAngulaire, ASSERVISSEMENT_VITESSE, 0.0);
    }
}

void MAE_MOUVEMENT_COMPLEXE_Termine(void * p)
{
    if (MAE_PremierPassage(&this->membres.mae))
    {
        this->etat.etatMouvement = DEPLACEMENT_MOUVEMENT_TERMINE;
        fprintf(stderr, "DEPLACEMENT | MOUVEMENT COMPLEXE | Termine\n");
        ASSERVISSEMENT_SetConsigne(this->membres.asservissementLongitudinal, ASSERVISSEMENT_POSITION, ODOMETRIE_GetPositionLongitudinale());
        ASSERVISSEMENT_SetConsigne(this->membres.asservissementAngulaire, ASSERVISSEMENT_POSITION, ODOMETRIE_GetPositionAngulaire());
    }
}

void MAE_MOUVEMENT_COMPLEXE_Stop(void * p)
{
    if (MAE_PremierPassage(&this->membres.mae))
    {
        this->etat.etatMouvement = DEPLACEMENT_MOUVEMENT_STOP;
        fprintf(stderr, "DEPLACEMENT | MOUVEMENT COMPLEXE | Stop\n");
    }
}

void MAE_MOUVEMENT_COMPLEXE_Dodo(void * p)
{
    return;
}


DEPLACEMENT_MOUVEMENT_ETAT MOUVEMENT_COMPLEXE_GetEtatDeplacement()
{
    return this->etat.etatMouvement;
}

void MOUVEMENT_COMPLEXE_Run()
{
    MAE_Automate(&this->membres.mae);
}