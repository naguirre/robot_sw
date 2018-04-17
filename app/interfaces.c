#include <app/interfaces.h>

#define CODEUR_NB_PAS       32
#define CODEUR_REDUCTEUR    1.0

DEPLACEMENT_INTERFACES interfaces;
static DEPLACEMENT_INTERFACES * this = &interfaces;

void DEPLACEMENT_INTERFACES_Init(float periode)
{
    static BOOL isInit = FALSE;
    
    if(!isInit)
    {
        CODEUR_Init(&this->codeurs[DEPLACEMENT_MOTEUR_DROITE], DEPLACEMENT_MOTEUR_DROITE, CODEUR_NB_PAS, CODEUR_REDUCTEUR, periode);
        CODEUR_Init(&this->codeurs[DEPLACEMENT_MOTEUR_GAUCHE], DEPLACEMENT_MOTEUR_GAUCHE, CODEUR_NB_PAS, CODEUR_REDUCTEUR, periode);
        PONT_H_Init(&this->pontsH[DEPLACEMENT_MOTEUR_DROITE], DEPLACEMENT_MOTEUR_DROITE);
        PONT_H_Init(&this->pontsH[DEPLACEMENT_MOTEUR_GAUCHE], DEPLACEMENT_MOTEUR_GAUCHE);
        isInit = TRUE;
    }
}

void DEPLACEMENT_INTERFACES_ActualiserEtat()
{
    CODEUR_ActualiserEtat(&this->codeurs[DEPLACEMENT_MOTEUR_DROITE]);
    CODEUR_ActualiserEtat(&this->codeurs[DEPLACEMENT_MOTEUR_GAUCHE]);
}

float DEPLACEMENT_INTERFACES_GetAngleBrut(DEPLACEMENT_MOTEUR moteur)
{
    return CODEUR_GetAngleBrut(&this->codeurs[moteur]);
}

void DEPLACEMENT_INTERFACES_SetAngleBrut(DEPLACEMENT_MOTEUR moteur, float angle)
{
    CODEUR_SetAngleBrut(&this->codeurs[moteur], angle);
}

float DEPLACEMENT_INTERFACES_GetVitesseAngulaire(DEPLACEMENT_MOTEUR moteur)
{
    return CODEUR_GetVitesseAngulaire(&this->codeurs[moteur]);
}

void DEPLACEMENT_INTERFACES_SetPuissance(DEPLACEMENT_MOTEUR moteur, float puissance)
{
    puissance = MATHS_Saturer(puissance, 1.0);
    PONT_H_ChangerPuissance(&this->pontsH[moteur], puissance);
}

