#ifndef __CODEUR_H__
#define __CODEUR_H__

#include <types.h>

#include "drivers/qei.h"

/*************************************/
/* HARDWARE MAPPING                  */
/*************************************/

//Module utilisé pour chaque codeur
static const QEI_CHANNEL QEI_CHANNELS[4] = {QEI0, QEI1, QEI2, QEI3};

/*************************************/
/* OBJET STRUCTURE                   */
/*************************************/

class CODEUR
{
public:
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

    CODEUR(uint8_t id, uint16_t nombrePasCodeur, float reducteur, float periode);

    void ActualiserEtat();

    float GetAngleBrut();

    void SetAngleBrut(float Val);

    float GetAngleReel();

    float GetVitesseAngulaire();
};

#endif /* __CODEUR_H__ */
