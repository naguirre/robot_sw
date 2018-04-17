#ifndef MAE_H
#define MAE_H

#include <types.h>
#include <tick/tick.h>

#define DERNIER_ETAT(a)    ETAT_ ## a
#define ETAT(a)            DERNIER_ETAT(a),

#define NB_ETATS(nom)  nom ## _NB_ETATS
#define FONCTIONS(nom) nom ## _Fonctions

#define CREATE_MAE_ENUM(nom, liste) \
enum                        \
{                           \
    liste,                  \
    NB_ETATS(nom)           \
};                          \

#define CREATE_MAE_FONCTIONS(nom, liste) void (*const FONCTIONS(nom)[])(void *) = {liste};

typedef void (*MAE_FONCTION)(void *);


typedef struct{
    uint8_t  etatActuel;
    uint8_t  etatPrecedent;
    uint32_t timer;                //horloge à l'arrivée dans l'état actuel
    uint32_t dureeTimeout;
    BOOL     premierPassage;        //premier passage dans l'état actuel
    BOOL     enTimeout;               //en timeout ?
    uint32_t dureeAttenteDebut;      //pour attente avant de commencer action
    BOOL     enAttenteDebut;
    
    MAE_FONCTION * fonctions;
    uint8_t  nbEtats;
    uint8_t  etatParDefaut;        //en cas d'état incohérent
    
    void * proprietaire;
}MAE;


void MAE_Init(MAE * this, uint8_t etatInit, uint32_t dureeAttenteDebut, uint32_t dureeTimeout, uint8_t etatParDefaut, uint8_t nbEtats,
              MAE_FONCTION * fonctions, void * proprietaire);

void MAE_Automate(MAE * this);

void MAE_ChangerEtat(MAE * this, uint8_t nouvelEtat, uint32_t dureeAttenteDebut, uint32_t dureeTimeout);

void MAE_EtatPrecedent(MAE * this);

BOOL MAE_AttenteDebutTerminee(MAE * this);

BOOL MAE_VerifierTimeout(MAE * this);

BOOL MAE_PremierPassage(MAE * this);

uint8_t MAE_GetEtatActuel(MAE * this);

uint8_t MAE_GetEtatPrecedent(MAE * this);

#endif
