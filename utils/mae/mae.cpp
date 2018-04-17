#include <mae/mae.h>

void MAE_Init(MAE * this, uint8_t etatInit, uint32_t dureeAttenteDebut, uint32_t dureeTimeout, uint8_t etatParDefaut, uint8_t nbEtats, MAE_FONCTION * fonctions, void * proprietaire)
{
    this->etatActuel         = etatInit;
    this->etatPrecedent      = etatInit;
    this->timer              = TICK_Get();
    this->dureeAttenteDebut  = dureeAttenteDebut;
    this->dureeTimeout       = dureeTimeout;
    this->fonctions          = fonctions;
    
    this->etatParDefaut      = etatParDefaut;
    this->nbEtats            = nbEtats;
    
    this->proprietaire       = proprietaire;
}

void MAE_Automate(MAE * this)
{
    //tempo de travail si besoin pour l automate
    if (TICK_Get() - this->timer > this->dureeAttenteDebut)
    {
        this->enAttenteDebut = FALSE;
    }
    
    if (TICK_Get() - this->timer > this->dureeTimeout)
    {
        this->enTimeout = TRUE;
    }
    
    //appel de la fonction correspondante a l etat courant
    if (this->etatActuel < this->nbEtats)
    {
        this->fonctions[this->etatActuel](this->proprietaire);      //appel de la fonction correspondant à l'état en cours
    }
    else
    {
        this->etatActuel = this->etatParDefaut; //au cas ou bug
    }
}

void MAE_ChangerEtat(MAE * this, uint8_t nouvelEtat, uint32_t dureeAttenteDebut, uint32_t dureeTimeout)
{
    if(nouvelEtat < this->nbEtats) //on verifie valeur correcte pour le nouvel etat
    {
        this->etatPrecedent     = this->etatActuel; // on sauvegarde l'etat courant
        this->etatActuel        = nouvelEtat; //on renseigne le nouvel etat
        this->premierPassage    = TRUE; //on positionne le booleen pour signifier le 1er acces a l etat
        this->timer             = TICK_Get();
        this->dureeAttenteDebut = dureeAttenteDebut;
        this->dureeTimeout      = dureeTimeout;
        this->enAttenteDebut    = TRUE;
        this->enTimeout         = FALSE;
    }
    else
    {
        this->etatActuel = this->etatParDefaut; //au cas ou valeur en dehors des etats connus
    }
}

void MAE_EtatPrecedent(MAE * this)
{
    uint8_t u8Tmp;
    
    //permutation entre etatPrecedent et etatActuel
    u8Tmp = this->etatPrecedent;
    this->etatPrecedent = this->etatActuel;
    this->etatActuel  = u8Tmp;
    this->premierPassage = TRUE;    //on positionne le booleen pour signifier le 1er acces a l etat
}

BOOL MAE_AttenteDebutTerminee(MAE * this)
{
    return !this->enAttenteDebut;
}

BOOL MAE_VerifierTimeout(MAE * this)
{
    return this->enTimeout;
}

BOOL MAE_PremierPassage(MAE * this)
{
    if(this->premierPassage)
    {
        this->premierPassage = FALSE;
        return TRUE;
    }else{
        return FALSE;
    }
}

uint8_t MAE_GetEtatActuel(MAE * this)
{
    return this->etatActuel;
}

uint8_t MAE_GetEtatPrecedent(MAE * this)
{
    return this->etatPrecedent;
}

