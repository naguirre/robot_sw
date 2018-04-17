#include <app/interfaces.h>

#define CODEUR_NB_PAS       32
#define CODEUR_REDUCTEUR    1.0

DEPLACEMENT_INTERFACES::DEPLACEMENT_INTERFACES(float periode)
{
    codeurs[DEPLACEMENT_MOTEUR_DROITE] = new CODEUR(DEPLACEMENT_MOTEUR_DROITE, CODEUR_NB_PAS, CODEUR_REDUCTEUR, periode);
    codeurs[DEPLACEMENT_MOTEUR_GAUCHE] = new CODEUR(DEPLACEMENT_MOTEUR_GAUCHE, CODEUR_NB_PAS, CODEUR_REDUCTEUR, periode);
    pontsH[DEPLACEMENT_MOTEUR_DROITE] = new PONT_H(DEPLACEMENT_MOTEUR_DROITE);
    pontsH[DEPLACEMENT_MOTEUR_GAUCHE] = new PONT_H(DEPLACEMENT_MOTEUR_GAUCHE);
}

void DEPLACEMENT_INTERFACES::ActualiserEtat()
{
    codeurs[DEPLACEMENT_MOTEUR_DROITE]->ActualiserEtat();
    codeurs[DEPLACEMENT_MOTEUR_GAUCHE]->ActualiserEtat();
}

float DEPLACEMENT_INTERFACES::GetAngleBrut(DEPLACEMENT_MOTEUR moteur)
{
    return codeurs[moteur]->GetAngleBrut();
}

void DEPLACEMENT_INTERFACES::SetAngleBrut(DEPLACEMENT_MOTEUR moteur, float angle)
{
    codeurs[moteur]->SetAngleBrut(angle);
}

float DEPLACEMENT_INTERFACES::GetVitesseAngulaire(DEPLACEMENT_MOTEUR moteur)
{
    return codeurs[moteur]->GetVitesseAngulaire();
}

void DEPLACEMENT_INTERFACES::SetPuissance(DEPLACEMENT_MOTEUR moteur, float puissance)
{
    puissance = MATHS_Saturer(puissance, 1.0);
    pontsH[moteur]->ChangerPuissance(puissance);
}
