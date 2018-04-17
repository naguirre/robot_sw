#ifndef CODEUR_H
#define CODEUR_H

#include <types.h>

#include <drivers/qei.h>

/*************************************/
/* HARDWARE MAPPING                  */
/*************************************/

//Module utilisé pour chaque codeur
static const QEI_CHANNEL QEI_CHANNELS[4] = {QEI0, QEI1, QEI2, QEI3};

/*************************************/
/* OBJET STRUCTURE                   */
/*************************************/

typedef struct{
    QEI  qei;   //Abstraction du périph QEI utilisé par le codeur
    struct{
        float    reducteur;      //Réducteur entre l'arbre de sortie du moteur et l'axe du codeur
        uint16_t nombrePas;      //Nombre de pas par tour du codeur
        float    facteurEchelle; //Rapport entre un pas de codeur et une portion d'angle
        float    periode;        //Période d'acquisition des données en secondes (pour calcul de vitesse)
    }parametres;
    struct{
        float    angleBrut;          //angle absolu de l'arbre du moteur en degrés
        float    angleReel;          //angle ramené à ]-180; 180]
        float    vitesseAngulaire;   //en degré/seconde
    }etat;
}CODEUR;

/*************************************/
/* FONCTIONS                         */
/*************************************/

void CODEUR_Init(CODEUR * this, uint8_t id, uint16_t nombrePasCodeur, float reducteur, float periode);

void CODEUR_ActualiserEtat(CODEUR * this);

float CODEUR_GetAngleBrut(CODEUR * this);

void CODEUR_SetAngleBrut(CODEUR * this, float Val);

float CODEUR_GetAngleReel(CODEUR * this);

float CODEUR_GetVitesseAngulaire(CODEUR * this);

#endif
