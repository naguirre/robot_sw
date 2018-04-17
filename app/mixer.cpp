#include <app/mixer.h>
#include <app/interfaces.h>

void MIXER_AppliquerCommandeDroite(float commande)
{
    DEPLACEMENT_INTERFACES_SetPuissance(DEPLACEMENT_MOTEUR_DROITE, commande);
}

void MIXER_AppliquerCommandeGauche(float commande)
{
    DEPLACEMENT_INTERFACES_SetPuissance(DEPLACEMENT_MOTEUR_GAUCHE, commande);
}

void MIXER_AppliquerCommandeDroiteGauche(float commandeDroite, float commandeGauche)
{
    MIXER_AppliquerCommandeDroite(commandeDroite);
    MIXER_AppliquerCommandeGauche(commandeGauche);
}

void MIXER_AppliquerCommandePolaire(float commandeLongitudinale, float commandeAngulaire)
{
    MIXER_AppliquerCommandeDroite(commandeLongitudinale + commandeAngulaire);
    MIXER_AppliquerCommandeGauche(commandeLongitudinale - commandeAngulaire);
}
