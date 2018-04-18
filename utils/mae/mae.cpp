#include <mae/mae.h>

MAE::MAE(uint8_t etatInit, uint32_t dureeAttenteDebut, uint32_t dureeTimeout, uint8_t etatParDefaut, uint8_t nbEtats, MAE_FONCTION * fonctions, void * proprietaire)
{
    etatActuel         = etatInit;
    etatPrecedent      = etatInit;
    timer              = TICK_Get();
    dureeAttenteDebut  = dureeAttenteDebut;
    dureeTimeout       = dureeTimeout;
    fonctions          = fonctions;

    etatParDefaut      = etatParDefaut;
    nbEtats            = nbEtats;

    proprietaire       = proprietaire;
}

void MAE::Automate(MAE * this)
{
    //tempo de travail si besoin pour l automate
    if (TICK_Get() - timer > dureeAttenteDebut)
    {
        enAttenteDebut = FALSE;
    }

    if (TICK_Get() - timer > dureeTimeout)
    {
        enTimeout = TRUE;
    }

    //appel de la fonction correspondante a l etat courant
    if (etatActuel < nbEtats)
    {
        fonctions[etatActuel](proprietaire);      //appel de la fonction correspondant à l'état en cours
    }
    else
    {
        etatActuel = etatParDefaut; //au cas ou bug
    }
}

void MAE::ChangerEtat(uint8_t nouvelEtat, uint32_t dureeAttenteDebut, uint32_t dureeTimeout)
{
    if(nouvelEtat < nbEtats) //on verifie valeur correcte pour le nouvel etat
    {
        etatPrecedent     = etatActuel; // on sauvegarde l'etat courant
        etatActuel        = nouvelEtat; //on renseigne le nouvel etat
        premierPassage    = TRUE; //on positionne le booleen pour signifier le 1er acces a l etat
        timer             = TICK_Get();
        dureeAttenteDebut = dureeAttenteDebut;
        dureeTimeout      = dureeTimeout;
        enAttenteDebut    = TRUE;
        enTimeout         = FALSE;
    }
    else
    {
        etatActuel = etatParDefaut; //au cas ou valeur en dehors des etats connus
    }
}

void MAE::EtatPrecedent(MAE * this)
{
    uint8_t u8Tmp;

    //permutation entre etatPrecedent et etatActuel
    u8Tmp = etatPrecedent;
    etatPrecedent = etatActuel;
    etatActuel  = u8Tmp;
    premierPassage = TRUE;    //on positionne le booleen pour signifier le 1er acces a l etat
}

BOOL MAE::AttenteDebutTerminee(MAE * this)
{
    return !enAttenteDebut;
}

BOOL MAE::VerifierTimeout(MAE * this)
{
    return enTimeout;
}

BOOL MAE::PremierPassage(MAE * this)
{
    if(premierPassage)
    {
        premierPassage = FALSE;
        return TRUE;
    }else{
        return FALSE;
    }
}

uint8_t MAE::GetEtatActuel(MAE * this)
{
    return etatActuel;
}

uint8_t MAE::GetEtatPrecedent(MAE * this)
{
    return etatPrecedent;
}
